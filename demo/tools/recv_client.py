IPC_TYPE = "SOCKET"
if IPC_TYPE == "SOCKET":
    import socket

    HOST = '127.0.0.1'
    PORT = 9000
    TRUNK_SIZE = 10240
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((HOST, PORT))
        while True:
            data = s.recv(TRUNK_SIZE).decode('ascii')
            if not data:
                print("Connection closed.")
                break
            print(data)
elif IPC_TYPE == "PIPE":
    import subprocess

    with subprocess.Popen("./server", stdout=subprocess.PIPE, text=True) as p:
        while True:
            data = p.stdout.readline()
            if not data:
                print("Connection closed.")
                break
            print(data, end='')
