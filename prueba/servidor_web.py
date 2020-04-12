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
                                if s[0].split(" ")[1] == "/":
                                    error = False
                                    codigo = "200 OK"
                                    nombre = "web.html"
                                    modo = "r"
                                    tipo = "text/html"
                                #elif s[0].split(" ")[1] == "/mundo.jpg":
                                #elif s[0].split(" ")[1] == "mundo.jpg":
                                elif ".jpg" in s[0].split(" ")[1]:
                                    error = False
                                    codigo = "200 OK"
                                    nombre = "mundo.jpg"
                                    modo = "rb"
                                    tipo = "image/jpg"
                                else:
                                    codigo = "404 Not found"
                                    tipo = "text/plain"
                                    datos = "Error"

                                for campo in s[1:]:
                                    if campo.split(" ", 1)[0] == "Connection:":
                                        # conexion = campo.split(" ", 1)[1]
                                        conexion = 'close'

                                if error == False:
                                    archivo = open(nombre, modo)
                                    datos = archivo.read()

                                elem.sendall((("HTTP/1.1 {}\r\nDate: {}\r\nServer: Apache\r\nContent-type: {}\r\nContent-leght: {}\r\nConnection: {}\r\n\r\n{}").format(codigo, str(datetime.date.today()), tipo, len(datos), conexion, '')).encode())

                                if nombre == "mundo.jpg":
                                    elem.sendall(datos)

                                elif nombre == "web.html":
                                    elem.sendall(datos.encode())

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