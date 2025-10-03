#include <ctype.h>  //для работы с чарами, isupper
#include <fcntl.h>  // open()
#include <stdio.h>  // стандартный ввод/вывод(printf(),scanf()) + работа с файлами(fopen(),fclose())
#include <stdlib.h>  //EXIT_FAILURE
#include <string.h>  //для работы со стороками
#include <unistd.h>  //доступ к pipe, fork и всем командам для ОС

#define BUFSZ 256

int main() {
    char buffer[BUFSZ];

    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        const char *msg = "Не удалось прочитать имя файла\n";
        write(STDERR_FILENO, msg, strlen(msg));
        return 1;
    }
    buffer[strcspn(buffer, "\n")] = '\0';

    int fd = open(buffer, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    // перенаправляем stdout в файл
    if (dup2(fd, STDOUT_FILENO) == -1) {
        perror("dup2");
        close(fd);
        return 1;
    }
    close(fd);

    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';

        if (isupper((unsigned char)buffer[0])) {
            printf("%s\n", buffer);
            fflush(stdout);
        } else {
            char err[BUFSZ];
            sprintf(err, "Error: строка должна начинаться с заглавной буквы - '%s'\n", buffer);
            write(STDERR_FILENO, err, strlen(err));
        }
    }

    return 0;
}
