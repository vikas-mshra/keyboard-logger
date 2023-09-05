# Custom Interrupt Handler for Keyboard Input - README

## Table of Contents

- [Introduction](#introduction)
- [Prerequisites](#prerequisites)
- [Instructions](#instructions)
- [Usage](#usage)
## Introduction

This README provides step-by-step instructions on how to create and use a custom interrupt handler for keyboard input on a Linux system. The custom handler will capture key presses and record them in a file named `input.txt`. This guide assumes that you have some familiarity with using the terminal on a Linux system.

## Prerequisites

Before you begin, make sure you have the following prerequisites:

- A Linux-based operating system.
- Administrative privileges or the ability to switch to the root user.
- Basic knowledge of using the terminal.

## Instructions

Follow these instructions to create and use the custom interrupt handler:

1. Open the terminal and navigate to the path where you want to work.

```bash
cd /path/to/your/directory
```

2. Switch to the root user. You can use the `sudo` command to execute commands with root privileges.

```bash
sudo su
```

3. Run the 'make' command to compile your custom interrupt handler code. This will generate several files, including `interrupt.o`, `interrupt.ko`, `interrupt.mod.c`, and `interrupt.mod.o`.

```bash
make
```

4. The 'insmod interrupt.ko' command will remove the existing interrupt handler and insert your custom handler.

```bash
insmod interrupt.ko
```

5. To verify that the command executed successfully, check the current directory. You should see a new `input.txt` file created. This file will be used to store all the keys pressed by the user.

6. With the custom interrupt handler in place, you are now ready to capture keyboard input. Simply press any key on the keyboard, and the keypresses will be recorded in the `input.txt` file.

**Note**: The text will only be visible in the `input.txt` file when you press the Enter key or reach the 128-character limit.

## Usage

You can now use this custom interrupt handler to capture and record keyboard input in real-time. To access the recorded keypresses, open the `input.txt` file located in the same directory where you executed the commands.

Feel free to customize the code and further enhance the functionality of the interrupt handler as needed for your specific use case.
