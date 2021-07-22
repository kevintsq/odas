import json
from threading import Thread, Condition
import socketserver


HOST = "localhost"
PORT = 9000
TRUNK_SIZE = 1024


class TrackInfo:
    def __init__(self):
        self.info: dict = None
        self.lock = Condition()
        self.updated = False
        self.finished = False

    def is_updated(self):
        return self.updated


class PotInfo:
    def __init__(self):
        self.info: dict = None
        self.lock = Condition()
        self.updated = False
        self.finished = False

    def is_updated(self):
        return self.updated


class ThreadingTCPRequestHandler(socketserver.BaseRequestHandler):

    def handle(self):
        while True:
            data = self.request.recv(TRUNK_SIZE)
            global track_info
            global pot_info
            if not data:
                print(f"Connection closed by {self.client_address}.")
                with track_info.lock:
                    track_info.updated = True
                    track_info.lock.notify()
                    track_info.finished = True
                with pot_info.lock:
                    pot_info.updated = True
                    pot_info.lock.notify()
                    pot_info.finished = True
                break
            # print(data.count(b"\n"))
            if data.count(b"\n") > 10:
                data = b"".join(data.split(b"\n")[-11:])
            try:
                result = json.loads(data)
            except:
                print("Bad format.")
                break
            if result["type"] == "tracks":
                with track_info.lock:
                    # print(result["src"])
                    track_info.info = result["src"]
                    track_info.updated = True
                    track_info.lock.notify()
            elif result["type"] == "pots":
                with pot_info.lock:
                    # print(result["src"])
                    pot_info.info = result["src"]
                    pot_info.updated = True
                    pot_info.lock.notify()
            else:
                print("Bad format.")
                break


class ThreadingTCPServer(socketserver.ThreadingMixIn, socketserver.TCPServer):
    pass


track_info = TrackInfo()
pot_info = PotInfo()


if __name__ == "__main__":
    with ThreadingTCPServer((HOST, PORT), ThreadingTCPRequestHandler) as server:
        server.allow_reuse_address = True
        # Start a thread with the server -- that thread will then start one more thread for each request
        server_thread = Thread(target=server.serve_forever, daemon=True)
        server_thread.start()
        print(f"Server loop running in thread: {server_thread.name}")
        while True:
            with track_info.lock:
                if track_info.finished:
                    break
                track_info.lock.wait_for(track_info.is_updated)
                print(track_info.info)
                track_info.updated = False
            with pot_info.lock:
                if pot_info.finished:
                    break
                pot_info.lock.wait_for(pot_info.is_updated)
                print(pot_info.info)
                pot_info.updated = False
        # Stops the serve_forever loop. Blocks until the loop has finished.
        server.shutdown()  # This must be called while serve_forever() is running in another thread, or it will deadlock.
        