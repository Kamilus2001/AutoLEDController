
import socket
from threading import Thread
from auto_brightness import AutoLED
from time import sleep
class Server:
    def __init__(self, ip="192.168.0.92", port=5001, buffer_size = 100, amount = 1):
        self.buffer_size = 1024
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.socket.bind((ip, port))
        self.conn = []
        self.addr = []
        self.amount = amount
        self.LED = AutoLED()
        self.LED.load()
    def wait_for_clients(self):
        self.socket.listen(self.amount)
        for i in range(0, self.amount):
            conn, addr = self.socket.accept()
            self.conn.append(conn)
            self.addr.append(addr)
            print("start for: {}".format(addr))
            Thread(target=self.start_listen, args=(conn, addr)).start()
    def start_listen(self, conn, addr):
        c = conn
        a = addr
        b  = 0
        while True:
            try:
                #do something with it
                data = c.recv(self.buffer_size)
                data = data.decode()
                print(data)
                data = float(data)
                b = self.LED.predict(data)
                b = str(b).encode()
                print(b)
                c.send(b)
                sleep(2)
            except Exception:
                pass
if __name__ == '__main__':
    S = Server()
    try:
        S.wait_for_clients()

    except Exception as e:
        print(e)
        S.socket.close()