#include "Header.h"

int main()
{
	char string[50];
	char command[50];
	char source[50];
	char destination[50];
	while (1)
	{
		int cindx = 0, source_indx = 0, dest_indx = 0, i = 0,count = 0;
		printf(">");
		scanf("%[^\n]%*c", string);
		string[strlen(string)] = '\0';
		while (string[i] != '\0')
		{
			while (string[i] != ' ' && string[i] != '\0' && string[i] != '\n')
			{
				command[cindx++] = string[i++];
			}
			command[cindx++] = '\0';
			if (string[i] != '\n' && i < strlen(string))
			{
				i++;
				while (string[i] != ' ' && string[i] != '\0' && string[i] != '\n')
				{
					source[source_indx++] = string[i++];
				}
				source[source_indx] = '\0';
				i++;
				while (string[i] != '\0')
				{
					destination[dest_indx++] = string[i++];
				}
				destination[dest_indx] = '\0';
			}
		}
		if (strcmp(command, "format") == 0)
		{
			format();
		}
		else if (strcmp(command, "ls") == 0)
		{
			list_all_files();
		}
		else if (strcmp(command, "copytodisk") == 0)
		{
			copy_to_disk(source, destination);
		}
		else if (strcmp(command, "copyfromdisk") == 0)
		{
			copy_from_disk(source, destination);
		}
		else if (strcmp(command, "exit") == 0)
		{
			exit(0);
		}
		else
			printf("invalid command\n");
	}
	system("pause");
}