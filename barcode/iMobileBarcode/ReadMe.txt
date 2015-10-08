URL: http://www.codeproject.com/Articles/5568/Serial-Communications-The-NET-Way

This project is for MOTO Barcode Reader

Serial Communications The .NET Way
Author: Timothy J. Krell (SAIC)

Overview

If you're like me, you have probably adopted the .NET framework as the basis for your latest Microsoft (MS) development efforts in an attempt to keep current on the platform. In my opinion, MS has done a very good job with the new framework and I especially like the new C# language, which is what this project is programmed in. 

Once you've learned your way around the assemblies, it quickly becomes apparent that MS, in their infinite wisdom, have totally ignored legacy serial ports and thus have not included any way to efficiently access and manage the ports in their native assemblies. This should come as no surprise - MS has always made it a pain to deal with serial ports. 

Since there was no way to efficiently and completely manage serial ports provided in the assemblies, I had to rely on the old Win32 library calls for the necessary interfaces. To make these function calls compatible with C# and OO methodology in general, they have all been packaged in wrapper classes whose access is limited to the container class.

The "Serial.sln" project contains a complete set of tools for the configuration and management of legacy COM ports. The Settings form provides a tool for setting up the COM port and saving the setup parameters to a user defined configuration file. The Terminal form provides a model for how the base class can be instantiated and how the delegates are defined and used to get access to the base overrides. The terminal is a very useful tool for implementing and testing the serial classes as well as testing any general serial communications.

Win32 Wrapper Classes

The Win32xxx files are internal wrapper classes that are used only by the SerialComm abstract class. This is done to provide complete encapsulation of the Win32 functions. As you will see, each of the Win32 function calls is private to the class and is contained within a wrapper method. It is this wrapper method that is called by the SerialComm class. This approach provides controlled access to the legacy functions. A brief summary of each class is as follows -

Win32Com - Contains the most basic com port interface functions - i.e. CreateFile() (open the port) CloseHandle() (close the port), ReadFile() (read from port), WriteFile() (write to port) CancelIo() (cancel all pending I/O), PurgeComm() (purge I/O buffers) and TransmitCommChar() (special char transmit).

Win32DCB - Contains the GetCommState() and SetCommState() functions that control access to the Device Control Block (DCB) and the properties that represent each member of the DCB control structure that is used to control serial port behavior.
Win32Enums - Contains a set of public enumerations that are used by all of the Win32 wrapper classes.
Win32Escape - Contains the EscapeCommFunction() function, which directs a specified communications device to perform an extended function.
Win32Events - Contains the GetCommMask(), SetCommMask() and WaitCommMask() functions which control event setup and handling by the receiver.
Win32Modem - Contains the GetCommModemStatus() function which returns the current states of the modem control register. This register indicates the current state of the CTS, DSR, RLSD and RING signals.
Win32Overlap - Contains the GetOverlappedResult() function and the Overlapped structure which provide a means to process serial I/O asynchronously.
Win32Properties - Contains the SetupComm() and GetCommProperties() functions which provide methods for reading and writing certain  port management properties - particularly the read/write queue sizes.
Win32Status - Contains the ClearCommError() function and port status structures.
Win32Timeouts - Contains the GetCommTimeouts() and SetCommTimeouts() functions and the Timeouts structure which control read/write timeout setup.

The SerialComm Abstract Class

SerialComm is an abstract class that must be inherited to get access to all of the public properties and methods that provide control over the serial port(s). It instantiates all of the Win32xxx wrapper classes and thus encapsulates them within the abstraction. This class exposes the methods and properties needed by the inheriting class to control the port.

SerialComm also contains a number of protected virtual event handlers - i.e. the OnError, OnBreak, etc. methods. By overriding these methods (as is done by the SerialBase class), the inheriting class can hook these functions to provide case specific functionality.

SerialComm also provides the receive thread that is automatically started when the port is opened and stopped when the port is closed. The receive thread handles any incoming data asynchronously from the transmitter. Overlapping is used to more efficiently handle asynchronous I/O.


The SerialConfig Class

SerialConfig is the basic port configuration class. It contains most of the members of the DCB as well as some other properties that control timeouts, queue sizes and other parameters. 

The Settings form usually handles actual manipulation of the contents of this class. This tool provides the user with a graphical means to configure the port. Not all members of the configuration class appear on the form.  In my implementation, those members that are generally managed by the OS are not included. The user can easily add these members to the form if so desired.

The SerialBase Class

SerialBase is the base interface class that inherits the SerialComm class, instantiates a SerialConfig class and provides a delegate structure that permits the user to get access to the protected virtual event handler methods from SerialComm. One SerialBase class is normally declared for each serial port that the user wishes to control. SerialBase can either be inherited or instantiated, as is done by the Terminal application. 

The Serial Test & Debug Terminal

The serial terminal application includes a TerminalMain class that contains the Main() method for starting the terminal. This means of starting the terminal is only used in this test implementation - application startup would normally be done elsewhere.

The terminal constructor demonstrates a typical implementation of how the serial base class is instantiated and how the delegate classes are utilized. On or about Lines 125 & 129, the source declares a SerialPort class and WithEvents structure, which are initialized at the end of the constructor. 

The end of the terminal file contains the delegate methods that point to the overrides from the base class. There is one for every virtual method currently declared, though not all may be necessary in all implementations. 

Feedback

As I am sure that most of you can appreciate, I have gone to a considerable amount of effort to put this project together and am giving it away free of charge to all interested parties in the hope that I will receive some constructive feedback. Since I do not, by any means, consider myself an expert on the new .NET framework (nor C# for that matter), I would greatly appreciate any comments on how useful you think this software is to you, suggestions for improvements to the overall architecture and/or the efficiency of the base classes, any bug reports, etc. 

To reach me, please send all comments to: tjkrell@comcast.net. 



Open Source License

THIS SOFTWARE PROGRAM ("PROGRAM") IS PROVIDED UNDER THE TERMS OF THIS SCIENCE APPLICATIONS INTERNATIONAL ("SAIC") PUBLIC LICENSE AGREEMENT ("AGREEMENT"). ANY USE, REPRODUCTION, OR DISTRIBUTION OF THE PROGRAM CONSTITUTES YOUR ACCEPTANCE OF THE TERMS OF THIS AGREEMENT.

Permission to use, copy, modify, and distribute the Program and its documentation, if any, for any purpose and without fee is hereby granted by SAIC to you, provided that:  (i) you not charge any fee for the Program, and the Program not be incorporated by you in any software or code for which compensation is expected or received; (ii) the copyright notice listed below appears in all copies; (iii) both the copyright notice and this Agreement appear in all supporting documentation; and (iv) the name of SAIC not be used in advertising or publicity pertaining to distribution of the Program without specific, written prior permission. 

THE PROGRAM IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OR CONDITIONS OF TITLE, NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.

You are solely responsible for determining the appropriateness of using and distributing the Program and you assume all risks associated with the exercise of rights under this Agreement, including but not limited to the risks and costs of program errors, compliance with applicable laws, damage to or loss of data, programs or equipment, and unavailability or interruption of operations.

EXCEPT AS EXPRESSLY SET FORTH IN THIS AGREEMENT, SAIC SHALL NOT HAVE ANY LIABILITY FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING WITHOUT LIMITATION LOST PROFITS), HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OR DISTRIBUTION OF THE PROGRAM OR THE EXERCISE OF ANY RIGHTS GRANTED HEREUNDER, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES. 

If any provision of this Agreement is invalid or unenforceable under applicable law, it shall not affect the validity or enforceability of the remainder of the terms of this Agreement, and without further action by the parties hereto, such provision shall be reformed to the minimum extent necessary to make such provision valid and enforceable.  It is your responsibility to comply with any export laws of your jurisdiction, including without limitation, the United States. This Agreement is governed by the laws of the State of California and the intellectual property laws of the United States of America. No party to this Agreement will bring a legal action under this Agreement more than one year after the cause of action arose. 

If you have any comments, suggestions, improvements, modifications, alterations, derivative works, or other changes (collectively "Contributions") to the Program that you would like to provide to SAIC, please send such Contributions to: tjkrell@comcast.net.  By sending Contributions to SAIC, you acknowledge and agree that you grant to SAIC a non-exclusive, worldwide, fully paid-up, royalty-free, license to reproduce, prepare derivative works of, publicly display, publicly perform, distribute and sublicense the Contributions, in source code and/or object code form, and such license is provided by you without any right of accounting.

The copyright notice for the Program is as follows:  Copyright (c) 1993 Science Applications International Corporation (SAIC).  All Rights Reserved.  Sponsored by the U.S. Government under contract #DAAH01-00-D-0013/10.

