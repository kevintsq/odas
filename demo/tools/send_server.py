IPC_TYPE = "SOCKET"
if IPC_TYPE == "SOCKET":
    import socket

    HOST = ''
    PORT = 9000
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR | socket.SO_REUSEPORT, 1)
        s.bind((HOST, PORT))
        s.listen(1)
        connection, address = s.accept()
        with connection:
            print(f'Connected by {address}.')
            count = 0
            while True:
                state = connection.send(f'{count}\n'.encode('utf-8'))
                if not state:
                    print(f"Connection closed from {address}.")
                    break
                count += 1
elif IPC_TYPE == "PIPE":
    import subprocess

    with subprocess.Popen("./client", stdin=subprocess.PIPE, text=True) as p:
        while True:
            state = p.stdin.write("Hello client from server!\n")
            if not state:
                print("Connection closed.")
                break
