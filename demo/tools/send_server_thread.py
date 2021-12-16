IPC_TYPE = "SOCKET"
if IPC_TYPE == "SOCKET":
    import socket
    import threading
    
    HOST = ''
    PORT = 9000
    CONNECTIONS = 2
    TRUNK_SIZE = 1024
    
    def handle_request(connection):
        with connection:
            count = 0
            while True:
                state = connection.send(f'{count}\r\n'.encode('utf-8'))
                if not state:
                    print(f"Connection closed from {address}.")
                    break
                print(count)
                count += 1
    
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR | socket.SO_REUSEPORT, 1)
        s.bind((HOST, PORT))
        s.listen(CONNECTIONS)
        threads = []
        for c in range(CONNECTIONS):
            connection, address = s.accept()
            print(f'Connected by {address}.')
            thread = threading.Thread(target=handle_request, args=(connection,), daemon=True)
            thread.start()
            threads.append(thread)
        for thread in threads:
            thread.join()
elif IPC_TYPE == "PIPE":
    import subprocess

    with subprocess.Popen("./client", stdout=subprocess.PIPE, text=True) as p:
        while True:
            data = p.stdout.readline()
            if not data:
                print("Connection closed.")
                break
            print(data, end='')
