# Web-Server-Final-Project
Autores: Del aula 212: Carla Pérez(nick), Francisco Vicente Suárez(@FVSB), Jan Carlos Pérez(ww@JCarlosPG).

Segundo Proyecto Sistemas Operativos, Facultad de Matemática y Computación, Universidad de La Habana.

Puntos a resolver en el Web Server: los # de la orientacion adjunta, se explicarán a continuación los puntos más importantes para la comprensión del proyecto. 

# 1)Ejecución del Web Server:
Para ejecutar el web server se deben pasar como parámetros el número del puerto y la ruta inicial.
## De esta manera:
### user@machine: ./WebServer.out \<port> \<directory>
## Por ejemplo:
  ### user@machine: ./WebServer.out 8000/home/lexie
  Listening port: 8000, 
  Serving directory: home/lexie
# 2)Conexión del servidor:
Después de comprabar la validez de los parámetros se procede a crear el socket por el cual el servidor va a escuchar a los clientes, el socket se crea
usando la función *socket* que devuelve el file descriptor(FD) del servidor.
Luego de crear el socket se configura el servidor usando *setsockopt* para
que este termine y se reinicie automáticamente, se le asigna una dirección y el
puerto mediante la función *bind* y finalmente se le indica al FD del
servidor que es un listening descriptor mediante la función *listen*.
Luego de esto se inicia un bucle donde se acepta la conexión usando la función *accept*, se lee el FD del cliente, se parse la uri de hexadecimal
a decimal, se reponde la petición del cliente y se cierra el FD del cliente._(aqui introducir lo de los hilos)_
# 3)Peticiones y respuestas(Requests and responses):
El servidor muestra los directorios y archivos que se encuentran en el serving directory y permite navegar entre los distintos directorios.

El servidor responde a peticiones de tipo GET y POST de HTTP, los demás métodos que no están implementados provocan una respuesta
¨Error 501 not implemented¨._(preguntar)_. La request de tipo GET se tiene cuando se hace click sobre el enlace de algún directorio
(genera un HTML con los subdirectorios) o sobre un archivo(descarga el archivo en el dispositvo cliente).

Las peticiones de tipo POST son enviadas al servidor cuando el clienta presiona algún botón de la página, estos sirven para ordenar los
directorios y archivos atendiendo a los diferentes criterios de ordenación implementados.

Para leer las peticiones del usuario se utiliza la función _read_ y para escribir la respuesta del servidor se emplea la función _write_, 
ambas definidas en el archivo _nombre del archivo.c_, estas tienen en cuenta los shortcounts y manejan el caso de interrupción 
de lectura/escritura por error EINTR. Para identificar si la uri de la petición corresponde a una carpeta o a un archivo se utiliza
la función _stat_ para saber si existe y después se analiza si es un directorio o un archivo usando S_ISDIR y se hace lo que especificó anteriormente.
Si _stat_ devolviera un valor menor que cero entonces se responde con "Error 404 Not Found".

Cuando S_ISDIR devuelve 1, entonces se procede a abrir el directorio con la función _opendir_ y se extrae la información de las subcarpetas
usando _readdir_ y esta se almacena en una _linked list(preguntar)_ que se encuentra en declarada en el archivo _archivo.h_, luego se genera un
documento HTML con toda la información recogida y el respectivo encabezado HTTP que indica al navegador cómo interpretar lo que está en el
HTML, luego ambos se escriben en el FD del cliente.

Cuando S_ISDIR devuelve 0 entonces es una petición de descarga, y para enviar el archivo se usa la función _sendfile_, inicialmemte se genera
el encabezado HTTP de descarga identificando el MIME-Type: application/octet-stream indicando que se va a realizar una descarga, 
se ecribe el encabezado en el FD del cliente y luego se manda el archivo.

Las request de método POST devuelven al usuario los subdirectorios del directorio especificado, pero ordenados con 
anterioridad atendiendo al criterio seleccionado, para esto procede exactamente igual que si se pidiera un directorio,
es decir se hace un HTML y un encabezado HTTP los cuales se escriben en el FD del cliente.














