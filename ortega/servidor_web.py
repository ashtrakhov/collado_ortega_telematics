import socket, sys, select, datetime

def main():
    if (len(sys.argv) != 2):
        print("Parametro <Puerto Servidor>")
        exit(1)

    sock_ser = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    lista = [sock_ser]

    sock_ser.bind(("127.0.0.1", int(sys.argv[1])))

    sock_ser.listen(1)

    try:
        while True:
            para_leer, para_escribir, error = select.select(lista, [], [], 300)

            if len(para_leer) == 0:
                print("Servidor web inactivo")

                for elem in lista[1:]:
                    elem.close()

                lista = [lista[0]]
            else:
                for elem in para_leer:
                    if elem == sock_ser:
                        sock_cli, sock_cliaddr = sock_ser.accept()
                        lista.append(sock_cli)
                    else:
                        info = elem.recv(4096).decode()

                        if info == "":
                            elem.close()
                            lista.remove(elem)
                        else:
                            s = info.split("\r\n")

                            if s[0].split(" ")[0] == "GET":
                                for campo in s[1:]:
                                    if campo.split(" ", 1)[0] == "Connection:":
                                        conexion = campo.split(" ", 1)[1]

                                if s[0].split(" ")[1] == "/":
                                    archivo = open("web.html", "r")
                                    datos = archivo.read()
                                    elem.sendall((("HTTP/1.1 200 OK\r\nDate: {}\r\nServer: Apache\r\nContent-type: text/html\r\nContent-length: {}\r\nConnection: {}\r\n\r\n{}").format(str(datetime.date.today()), len(datos), conexion, datos)).encode())
                                elif ".jpg" in s[0].split(" ")[1]:
                                    archivo = open(s[0].split(" ")[1][1:], "rb")
                                    datos = archivo.read()
                                    elem.sendall((("HTTP/1.1 200 OK\r\nDate: {}\r\nServer: Apache\r\nContent-type: image/jpg\r\nContent-length: {}\r\nConnection: {}\r\n\r\n").format(str(datetime.date.today()), len(datos), conexion)).encode() + datos)
                                else:
                                    datos = "Error"
                                    elem.sendall((("HTTP/1.1 404 Not found\r\nDate: {}\r\nServer: Apache\r\nContent-type: text/plain\r\nContent-length: {}\r\nConnection: {}\r\n\r\n{}").format(str(datetime.date.today()), len(datos), conexion, datos)).encode())

                                archivo.close()

                                if conexion == "close":
                                    elem.close()
                                    lista.remove(elem)
    except KeyboardInterrupt:
        for elem in lista:
            elem.close()

        exit(0)

if __name__ == "__main__":
    main()