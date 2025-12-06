import socket

HOST = ""           # accetta connessioni da ogni IP
PORT = 8080         # porta del server

html_page = """\
HTTP/1.1 200 OK
Content-Type: text/html; charset=utf-8

<html>
<head><title>Test Pagina</title></head>
<body>
    <h1>Benvenuto nella mia pagina HTML!</h1>
    <p>Questa pagina è inviata dal server Python.</p>
</body>
</html>
"""

# Creazione socket server
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind((HOST, PORT))
server_socket.listen(1)

print(f"Server in ascolto sulla porta {PORT}...")

while True:
    client_socket, client_addr = server_socket.accept()
    print("Connessione da:", client_addr)

    # ricevo la richiesta HTTP
    request = client_socket.recv(1024).decode()
    print("Richiesta ricevuta:\n", request)

    # invio la pagina HTML
    client_socket.sendall(html_page.encode("utf-8"))

    client_socket.close()
