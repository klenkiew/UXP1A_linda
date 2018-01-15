import random
import subprocess
import threading
from collections import namedtuple

input_file_name = 'tasks'
tuple_space_name = 'tuple_space_test' + str(random.uniform(0, 100000))
read_program_name = './LindaRead'
input_program_name = './LindaInput'
output_program_name = './LindaOutput'
timeout_program_option = '--timeout'
tuple_space_name_program_option = '--tuple_space'


class Command:
    def __init__(self, program_name, args):
        self.program_name = program_name
        self.args = args

    def __str__(self):
        return self.program_name + ' ' + str.join(' ', map(lambda arg: str(arg), self.args))


def get_command(task):
    program_name = get_program_name(task)
    args = [task.tuple, tuple_space_name_program_option + ' ' + tuple_space_name]
    if task.timeout is not None:
        args.append(timeout_program_option + ' ' + task.timeout)
    return Command(program_name, args)


def get_program_name(task):
    if task.operation == "R":
        return read_program_name
    elif task.operation == "I":
        return input_program_name
    elif task.operation == "O":
        return output_program_name


def run_command(command):
    command_str = str(command)
    print("Executing command " + command_str + " ...")
    subprocess.call([command_str], shell=True)
    print("Command " + command_str + " executed successfully")


def main():
    Task = namedtuple('Task', 'delay operation tuple timeout')
    tasks = []
    # input file structure:
    # {delay} [R/I/O] {tuple} {timeout}?
    with open(input_file_name, 'r') as input_file:
        for line in input_file.readlines():
            # comments start with '#'
            if line[0] == '#':
                continue

            parts = line.split()
            delay = int(parts[0])
            operation = parts[1]
            tuple = parts[2]
            timeout = parts[3] if len(parts) > 3 else None
            tasks.append(Task(delay, operation, tuple, timeout))

    commands_with_delay = []
    for task in tasks:
        command = get_command(task)
        commands_with_delay.append((command, task.delay))

    for command in commands_with_delay:
        threading.Timer(command[1], run_command, [command[0]]).start()

    print("All commands timers started.")


if __name__ == "__main__":
    main()
