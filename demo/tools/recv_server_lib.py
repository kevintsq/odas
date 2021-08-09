import json
from threading import Thread, Condition
import socketserver


HOST = "localhost"
PORT = 9000
TRUNK_SIZE = 1024


class ThreadingTCPRequestHandler(socketserver.BaseRequestHandler):

    def handle(self):
        while True:
            data = self.request.recv(TRUNK_SIZE)
            if not data:
                print(f"Connection closed by {self.client_address}.")
                break
            print(data)


class ThreadingTCPServer(socketserver.ThreadingMixIn, socketserver.TCPServer):
    pass


if __name__ == "__main__":
    with ThreadingTCPServer((HOST, PORT), ThreadingTCPRequestHandler) as server:
        server.allow_reuse_address = True
        # Start a thread with the server -- that thread will then start one more thread for each request
        server_thread = Thread(target=server.serve_forever, daemon=True)
        server_thread.start()
        print(f"Server loop running in thread: {server_thread.name}...")

        server.shutdown()  # This must be called while serve_forever() is running in another thread, or it will deadlock.
    print("Done.")
