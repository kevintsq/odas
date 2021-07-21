IPC_TYPE = "SOCKET"
if IPC_TYPE == "SOCKET":
    import socket
    import threading
    
    HOST = ''
    PORT = 9000
    CONNECTIONS = 2
    TRUNK_SIZE = 10240
    
    def handle_request(connection):
        with connection:
            while True:
                data = connection.recv(TRUNK_SIZE).decode('ascii')
                if not data:
                    print(f"Connection closed from {address}.")
                    return
                print(data)
    
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
