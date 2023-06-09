
    while (1)
    {
        // Aceptar clientes
        int connected_fd = accept(listenfd, &client_socket, &client_len);
        if (fork() == 0)
        {

            close(listenfd);

#pragma region Server

            buff = ReadRequest(connected_fd, size);
            // Separar Salida en 3 cadenas
            sscanf(buff, "%s %s %s", method, uri, version);

            uri = Uri_parser(uri);
            buff = NULL;
            // Si es la misma dirección que se paso como entrada
            if (strcmp(uri, orig_path) != 0)
            {
                // última aparación del /
                button = strrchr(uri, '/');
            }
            else
                button = uri;
            // Si el tipo de petición es Get
            if (strcmp(method, "GET") == 0)
            {
                // Piensa si estas en la carpeta raiz
                if (strcmp(uri, "/") == 0)
                { // Por defecto se entraria a la raiz que se introdujo al iniciar el programa
                    uri = strcpy(uri, orig_path);
                }
                struct stat folder;
                // Rellenar la estructura
                if (stat(uri, &folder) == 0)
                {
                    // Comprobar que sea carpeta
                    if (S_ISDIR(folder.st_mode))
                    {
                        // Inicializa
                        list = Initialize(uri, &list_len);
                        // Genera el html con la respuesta
                        HTMLresponse(list, uri, connected_fd, button, '0');
                    }
                    else // En caso de ser un archivo que se pueda descargar
                    {
                        Download(connected_fd, uri, folder.st_size);
                    }
                }
                else if (strstr(uri, orig_path) != NULL)
                {
                    HTMLerror(connected_fd, uri, "404", "Not Found", "Web Server could'n find this file");
                }
            }
            // Caso petición "Post"
            else if (strcmp(method, "POST") == 0)
            {
                char *p = strchr(uri, '~');
                char *c = strchr(p + 1, '~');
                *c = '\0';
                c++;
                int opt = (int)*(p + 1) - 48;
                opt += (((int)*(c)-48) % 2);
                *p = '\0';
                list = Initialize(uri, &list_len);
                SortBy(&list, list_len, comparer[opt]);
                char nxt = '1';
                if (opt % 2)
                    nxt = '0';
                HTMLresponse(list, uri, connected_fd, button, nxt);
            }
            else
            {
                HTMLerror(connected_fd, method, "501", "Not Implemented", "Web Server doesn't implement this method");
            }
#pragma endregion
