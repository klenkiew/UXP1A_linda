import random
import subprocess
import threading
import sys
from collections import namedtuple

input_file_name = 'tasks'
tuple_space_name = 'tuple_space_test' + str(random.uniform(0, 100000))
read_program_name = './LindaRead'
input_program_name = './LindaInput'
output_program_name = './LindaOutput'
timeout_program_option = '--timeout'
tuple_space_name_program_option = '--tuple_space'


# quick & dirty solution - but w/e
no_log = False


class Command:
    def __init__(self, program_name, args, redirect_stdout_to=None):
        self.program_name = program_name
        self.args = args
        self.redirect_stdout_to = redirect_stdout_to

    def __str__(self):
        return self.program_name + ' ' + str.join(' ', map(lambda arg: str(arg), self.args))


def get_command(task):
    program_name = get_program_name(task)
    args = [task.tuple, tuple_space_name_program_option + ' ' + tuple_space_name]
    if task.timeout is not None:
        args.append(timeout_program_option + ' ' + task.timeout)
    if no_log:
        args.append("-l fatal")
    stdout = task.stdout
    return Command(program_name, args, stdout)


def get_program_name(task):
    if task.operation == "R":
        return read_program_name
    elif task.operation == "I":
        return input_program_name
    elif task.operation == "O":
        return output_program_name


def run_command(command):
    command_str = str(command)
    print("[Script runner] Executing command " + command_str + " ...")
    stdout = get_stdout(command)
    subprocess.call([command_str], shell=True, stdout=stdout)
    print("[Script runner] Command " + command_str + " executed successfully")
    if stdout is not None:
        stdout.close()


def get_stdout(command):
    stdout = None
    if command.redirect_stdout_to is not None:
        stdout = open(command.redirect_stdout_to, 'w')
    return stdout


def main():
    handle_arguments()

    Task = namedtuple('Task', 'delay operation tuple timeout stdout')
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
            stdout = parts[4] if len(parts) > 4 else None
            tasks.append(Task(delay, operation, tuple, timeout, stdout))

    commands_with_delay = []
    for task in tasks:
        command = get_command(task)
        commands_with_delay.append((command, task.delay))

    for command in commands_with_delay:
        threading.Timer(command[1], run_command, [command[0]]).start()

    print("All commands timers started.")


def handle_arguments():
    # ugly solution with globals - :(
    global no_log
    global input_file_name
    if len(sys.argv) > 1:
        for arg in sys.argv[1:]:
            if arg == "--no_log":
                no_log = True
            else:
                input_file_name = arg


if __name__ == "__main__":
    main()