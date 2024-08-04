| **Program name** | minishell |
|------------------|-----------|
| **Turn in files**| Makefile, *.h, *.c |
| **Makefile**     | NAME, all, clean, fclean, re |
| **Arguments**    | - |
| **External functions** | readline, rl_clear_history, rl_on_new_line, rl_replace_line, rl_redisplay, add_history, printf, malloc, free, write, access, open, read, close, fork, wait, waitpid, wait3, wait4, signal, sigaction, sigemptyset, sigaddset, kill, exit, getcwd, chdir, stat, lstat, fstat, unlink, execve, dup, dup2, pipe, opendir, readdir, closedir, strerror, perror, isatty, ttyname, ttyslot, ioctl, getenv, tcsetattr, tcgetattr, tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs |
| **Libft authorized** | Yes |
| **Description**  | Write a shell |


### Shell Requirements

| Status | Requirement |
|--------|-------------|
| ✅ | Display a prompt when waiting for a new command. |
| ✅ | Have a working history. |
| ✅ | Search and launch the right executable (based on the PATH variable or using a relative or an absolute path). |
| ✅ | Avoid using more than one global variable to indicate a received signal. |
| ✅ | Not interpret unclosed quotes or special characters which are not required by the subject such as \ (backslash) or ; (semicolon). |
| ✅ | Handle ’ (single quote) which should prevent the shell from interpreting the metacharacters in the quoted sequence. |
| ✅ | Handle " (double quote) which should prevent the shell from interpreting the metacharacters in the quoted sequence except for $ (dollar sign). |
| ✅ | Implement redirections: |
| ✅ | ◦ < should redirect input. |
| ✅ | ◦ > should redirect output. |
| ✅ | ◦ << should be given a delimiter, then read the input until a line containing the delimiter is seen. However, it doesn’t have to update the history! |
| ✅ | ◦ >> should redirect output in append mode. |
| ✅ | Implement pipes. The output of each command in the pipeline is connected to the input of the next command via a pipe. |
| ✅ | Handle environment variables ($ followed by a sequence of characters) which should expand to their values. |
| ✅ | Handle $? which should expand to the exit status of the most recently executed foreground pipeline. |
| ✅ | Handle ctrl-C, ctrl-D and ctrl-\ which should behave like in bash. |
| ✅ | In interactive mode: |
| ✅ | ◦ ctrl-C displays a new prompt on a new line. |
| ✅ | ◦ ctrl-D exits the shell. |
| ✅ | ◦ ctrl-\ does nothing. |
| ✅ | Your shell must implement the following builtins: |
| ✅ | ◦ echo with option -n |
| ✅ | ◦ cd with only a relative or absolute path |
| ✅ | ◦ pwd with no options |
| ✅ | ◦ export with no options |
| ✅ | ◦ unset with no options |
| ✅ | ◦ env with no options or arguments |
| ✅ | ◦ exit with no options |

## EXPLICACION DEL CÓDIGO

- ¿Para qué una variable global?
    Dado que se trata de un proyecto monohilo se decide usar una variable global para almacenar el valor de salida de los comandos `g_status`. De esta forma compartimos su valor de forma consistente evitando tener que usar un *int que habría que pasar a todas las funciones.
- Arquitectura del proyecto
    Este proyecto tiene dos partes principales: Lexer (filtrado de datos de entrada) y executor (ejecución de comando previamente filtrado).

    El proyecto inicializa los datos de t_minishell (env y tokens) y data (cmd, path, cmd, heredoc, nodos) para luego pasar a la función principal, llamada `ft_program`
   
    Una vez empieza el bucle infinito ft_program se hacen 4 cosas principales:
        1. Gestionar las señales
        2. Recibir los datos de usuario mediante la librería readline (cuyo aux pondremos en rojo para da carisma).
        3. Depurar datos mediante las funciones de Lexer
        4. Ejecutar comandos
     1. La gestión de señales se realiza en el ficher `minishell_signals.c`.
        Utiliza las funciones rl_replace_line, remplaza linea actual con una cadena vacía, rl_on_new_line, para indicar a readline que el usuario ha comenzado una nueva linea, y rl_redisplay, que utilza el prompt de readline para mostra la línea actual.

        Además usa SIGINT y SIGQUIT (ignorada) para para gestionar las señales CTRL+C y CTRL+\
        CTRL+D se interpreta como un EOF e interrumpe la shell en ft_get_user_input
     2. La función readline es tan directa que no necesita de explicación
     3. El lexer tiene los siguientes propósitos:
        1. `add_history()`: Añadir el string al history con la funcionalidad de `readline()`'
        2. `ft_variable_expansion_check()`: Expandir las variables de entorno. Si se usan comillas simples se interpretaran como literales, pero al usar comillas dobles o no usar comillas se expandirá el valor de sus variables
        3. `ft_get_tokens()`: Clasificar el string de entrada en tokens delimitados por espacios, comillas dobles, comillas simples o pipes. Se realiza el trimado de espacios y tabuladores adicionales.
        4. `check_invalid_pipe()`: Chequear que la sintaxis de los pipes sea correcta: que conduzcan a algun sitio
        5. `ft_redirection()`: Inicializar la estructura data insertando los valores de las redirecciones, de haberlas, en el char **redirections. Los pipes definen la separación entre nodos de la lista enlazada que es data, por lo que habrá tantos nodos como `num_pipes + 1`
        6. `ft_commands()`: Añadir los comandos en el char **cmd de cada nodo
        7. `check_invalid_redirection()`: Chequea que la sintaxis de las redirecciones sea correcta:   
           - Redirección mal formada con redirecciones consecutivas
           - Redirección incompleta por falta de argumento o archivo tras ella
        8. `fill_cmd_path()`: Añadir la ruta al comando en la variable char *path de cada nodo. En caso de que sea un builtin o no se encuentre se deja vacío
     4.  El exec tiene las siguientes funcionalidades:
        1. `ft_init_fd()`: Inicializa la estructura de fd-s que gestionaran la comunicación entre nodos, inicializando los valores a -2 para indicar que no han sido configurados aún.
        2. `ft_count_nodes()`: Cuenta el número de nodos que el lexer a creado
        3. `ft_fill_here_doc()`: Rellena el campo heredoc de la estructura data a -2 y busca `<<`. Si lo encuentra lanza la función `heredoc()`.
        Hará un proceso hijo que lanza `double_redirection()`, el cual hace una gestión de la entrada de usuario con readline. Cuando se lee la key establecida en el heredoc se termina la entrada de datos de usuario (`command < key`)
        El proceso padre espera la ejecución del hijo y sobrescribe el valor de int heredoc con el fd de entrada.
        4. `ft_single_builtin()`: Se evalua si el comando del nodo es un builtin o no. En caso de que lo sea se llama a `ft_call_builtin()` y se compara el string del comando con los literales de cada builtin. Si coincide se ejecuta el builtin, si no sigue el código.
        `ft_dup_work()` gestiona la redicción de archivos ajustando los descriotres de archivo estandar fd_in y fd_out
        `ft_dups()` Hace la gestion de las redirecciones. Se añaden defines para que los modos de las mismas sean más entendibles. Esta función llama a su vez a `ft_get_fd()` que a su vez se encarga de gestionar el cierre y sobrescritura de cada fd para el caso oportuno.
        5. `ft_exec_loop()`: 
            `ft_signals_in_cat()`: Primero gestiona las señales en caso de que sea un cat, ya que este es un proceso bloqueante y las señales SIGINT y SIGQUIT se comportan diferente. Ctrl+C corta el comando e introduce un salto de linea y Ctrl+\ además pone el texto `Quit`

            Hace un fork del proceso actual y empieza un loop que recorre todos los nodos y va lanzando sus comandos.
            `ft_check_cmd()`: Comprueba si el comando tiene path asignado o es un builting. Si no tiene path asignado comprueba que no sea el comando exit y añade el valor de comando no encontrado al g_status, además de mostrar un mensaje de error. Tambien resetea los fd-s.
        Se usa `WIFEXITED` para saber si el comando ha salido con algun error y consultarlo en `WEXITSTATUS`.
        Se cierran los fd-s y se vuelve al main para liberar la memoria asignada a los nodos y volver a capturar el input de usuario
    5. BUILTINS
        Separamos los builtins de la parte del exe por tener entidad propia en el enunciado:
        - `echo`: Imprime los argumentos separados por espacio y añade un \n. Con el argumento -n no imprime \n.
        Procesa variables como `~` o `~/` (imprime el valor de $HOME con y sin barra) y `$?` (imprime el valor de g_status)
        - `cd`: Actualiza el directiorio actual y actualiza las variables de entorno PWD y OLDPWD. 
        Gestiona `~` para volver al HOME del usuario y `-` para volver al directorio previo en `ft_chdir_prev`
        Comprueba con la función estandar `access` si el directorio existe (F_OK).
        Guarda el directorio actual en `OLDPWD` y va al nuevo con chdir. Conseguimos la nueva ruta absoluta con `getcwd` y actualizamos `PWD`.
        - `pwd`: Usa la función de la librería estandar `getcwd` para mostrar el directorio actual
        - `export`: Actualiza o añade variables de entorno y muestra las variables de entorno si no lleva argumentos
        - `unset`: Hace un realloc de env eliminando la variable especificada y copiando los valores restantes al nuevo array (saltandose la que se elimina)
        - `env`: Devuelve las variables de entorno
        - `exit`: Gestiona la salida del programa y admite un argumento numérico para su código de salida (sin argumentos devuelve 0). Si el argumento no es valido (int) o tiene un | devuelve un error
