# JX11J

This project is built using the JUCE framework. JUCE is an open-source cross-platform C++ application framework used for the development of desktop and mobile applications.

## Table of Contents

- [Description](#description)
- [Prerequisites](#prerequisites)
- [Running](#running)
- [Acknowledgements](#acknowledgements)

## Description
This is a Synthesizer Plug-in based on the JX10 and JX11 from the book Creating Audio Synthesizer Plug-Ins with C++ and JUCE by Matthijs Hollemans.

## Prerequisites

Before you begin, ensure you have met the following requirements:

- **JUCE**: Download and install JUCE from [JUCE](https://juce.com/get-juce).
- **C++ Compiler**: Ensure you have a C++ compiler installed. For example:
  - On Windows: Visual Studio
  - On macOS: Xcode
  - On Linux: GCC
- **CMake**: This project uses CMake for building. Install CMake from [CMake](https://cmake.org/download/).

## Running Projucer
1. Open Projucer from the JUCE installation directory.

2. Open your .jucer project file:

    - Go to File > Open...
    - Navigate to your project directory and select the .jucer file.
3. Configure the project settings:
    - Set your Modules path to the JUCE modules folder.
    - Ensure the Exporter settings (such as Visual Studio, Xcode, or Linux Makefile) are configured correctly.
4. Save and export the project:
    - Click Save Project to save any changes.
    - Click Save and Open in IDE to generate the project files for your IDE (Visual Studio, Xcode, etc.).

## Acknowledgements
- [JUCE](https://juce.com/) - The framework used in this project.
- [CMake](https://cmake.org/) - Build system generator.
- [Book](https://www.theaudioprogrammer.com/learn/synth-plugin-book) - The book used to create this plugin
