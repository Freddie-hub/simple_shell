#include "shell.h"

/**
 * check_for_chain_delimiter - check if the current char in buffer is a chain delimiter
 * @info: the parameter struct
 * @buffer: the char buffer
 * @position: address of the current position in buffer
 *
 * Return: 1 if chain delimiter, 0 otherwise
 */
int check_for_chain_delimiter(info_t *info, char *buffer, size_t *position)
{
	size_t current_pos = *position;

	if (buffer[current_pos] == '|' && buffer[current_pos + 1] == '|')
	{
		buffer[current_pos] = 0;
		current_pos++;
		info->cmd_buf_type = CMD_OR;
	}
	else if (buffer[current_pos] == '&' && buffer[current_pos + 1] == '&')
	{
		buffer[current_pos] = 0;
		current_pos++;
		info->cmd_buf_type = CMD_AND;
	}
	else if (buffer[current_pos] == ';') /* found end of this command */
	{
		buffer[current_pos] = 0; /* replace semicolon with null */
		info->cmd_buf_type = CMD_CHAIN;
	}
	else
		return (0);

	*position = current_pos;
	return (1);
}

/**
 * should_continue_chaining - checks whether to continue chaining based on the last status
 * @info: the parameter struct
 * @buffer: the char buffer
 * @position: address of the current position in buffer
 * @start_pos: starting position in buffer
 * @length: length of buffer
 *
 * Return: Void
 */
void should_continue_chaining(info_t *info, char *buffer, size_t *position, size_t start_pos, size_t length)
{
	size_t current_pos = *position;

	if (info->cmd_buf_type == CMD_AND)
	{
		if (info->status)
		{
			buffer[start_pos] = 0;
			current_pos = length;
		}
	}
	if (info->cmd_buf_type == CMD_OR)
	{
		if (!info->status)
		{
			buffer[start_pos] = 0;
			current_pos = length;
		}
	}

	*position = current_pos;
}

/**
 * replace_alias_with_value - replaces an alias in the tokenized string with its value
 * @info: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int replace_alias_with_value(info_t *info)
{
	int i;
	list_t *alias_node;
	char *value;

	for (i = 0; i < 10; i++)
	{
		alias_node = find_alias_node(info->alias, info->argv[0], '=');
		if (!alias_node)
			return (0);

		free(info->argv[0]);
		value = _strchr(alias_node->str, '=') + 1;
		info->argv[0] = _strdup(value);
	}

	return (1);
}

/**
 * replace_variables_with_values - replaces variables in the tokenized string with their values
 * @info: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int replace_variables_with_values(info_t *info)
{
	int i = 0;
	list_t *var_node;

	for (i = 0; info->argv[i]; i++)
	{
		if (info->argv[i][0] != '$' || !info->argv[i][1])
			continue;

		if (!_strcmp(info->argv[i], "$?"))
		{
			char *status_value = convert_number(info->status, 10, 0);
			replace_string(&(info->argv[i]), _strdup(status_value));
			free(status_value);
			continue;
		}

		if (!_strcmp(info->argv[i], "$$"))
		{
			char *pid_value = convert_number(getpid(), 10, 0);
			replace_string(&(info->argv[i]), _strdup(pid_value));
			free(pid_value);
			continue;
		}

		var_node = find_alias_node(info->env, &info->argv[i][1], '=');
		if (var_node)
		{
			char *value = _strchr(var_node->str, '=') + 1;
			replace_string(&(info->argv[i]), _strdup(value));
			continue;
		}

		replace_string(&info->argv[i], _strdup(""));
	}

	return (0);
}

/**
 * replace_string_with_new - replaces string
 * @old_string: address of old string
 * @new_string: new string
 *
 * Return: 1 if replaced, 0 otherwise
 */
int replace_string_with_new(char **old_string, char *new_string)
{
	free(*old_string);
	*old_string = new_string;
	return (1);
}

