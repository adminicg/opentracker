 /* ========================================================================
  * Copyright (c) 2006,
  * Institute for Computer Graphics and Vision
  * Graz University of Technology
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted provided that the following conditions are
  * met:
  *
  * Redistributions of source code must retain the above copyright notice,
  * this list of conditions and the following disclaimer.
  *
  * Redistributions in binary form must reproduce the above copyright
  * notice, this list of conditions and the following disclaimer in the
  * documentation and/or other materials provided with the distribution.
  *
  * Neither the name of the Graz University of Technology nor the names of
  * its contributors may be used to endorse or promote products derived from
  * this software without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
  * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
  * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
  * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
  * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
  * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
  * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
  * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** Configuration include file for OpenTracker
  *
  * This file sets defines that control the compilation of device drivers in
  * OpenTracker. Some device drivers require additional libraries and might
  * not be available on all platforms. These source files are bracketed by
  * conditional compile clauses to only be compiled in, if desired. Edit this
  * file to set your configuration. This should only be changed, if you
  * recompile the library, as the include files that you use for other
  * projects will also require this file.
  * @author Gerhard Reitmayr
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page config Configuration Options
 *
 * Some device drivers require additional libraries and might
 * not be available on all platforms. These source files are bracketed by
 * conditional compile clauses to only be compiled in, if desired. You can select
 * these by enabling the defines in the file config_win.h. Edit this
 * file to set your configuration. This should only be changed, if you
 * recompile the library, as the include files that you use for other
 * projects will also require this file. Note that you
 * need to set the correct paths in the build system yourself, if automatic
 * discovery is not possible. This is necessary for VisualC++ where you will have
 * to modify your project settings. For Unices we are working on autotools support
 * and will include it there as far as possible.
 *
 * The following defines are currently available :
 * @li @b USE_ARTOOLKIT
 *      \n enables ARToolkit support and the @ref artoolkitsource node and @ref artoolkitmodule
 * @li @b USE_CYBERMOUSE
 *      \n enables Logitech Cybermouse support and the @ref cybermousesource. This works only on
 *      Windows.
 * @li @b USE_WACOMGRAPHIRE
 *		\n enables Wacom Tablet support and the @ref wacomgraphiresource. This works only
 *      on Windows.
 * @li @b USE_JOYSTICK
 *		\n enables joystick support via DirectInput and the @ref joysticksource. This works
 *      only on Windows.
 * @li @b USE_SPACEMOUSE
 *		\n enables SpaceMouse support and the @ref spacemousemodule. This works only
 *      on Windows.
 * @li @b USE_SAPISPEECH
 *		\n enables Speech Recognition and TextToSpeech support and the @ref speechmodule and
 *      @ref speechrecosource. By default only an empty implementation is present. This
 *      define enables support via the Microsoft SpeechAPI and works only under Windows.
 * @li @b USE_P5GLOVE
 *		\n enables support for Essentialreality's P5 glove using the @ref p5glovemodule and
 *       @ref p5glovesource. Support for this is only available on windows yet.
 *       Linux support is under development.
 * @li @b USE_DWARF
 *		\n enables interoperability of DWARF and OpenTracker. Using the @ref dwarfmodule
 *       allows you configure an OpenTracker process as a DWARF service where @ref
 *       dwarfsink act as abilities and @ref dwarfsource act as needs.
 * @li @b USE_VRPN
 *      \n enables interoperability with VRPN. OpenTracker can act both as a server and
 *       as a client to other VRPN processes. There is @ref vrpnmodule which allows setting
 *       the server parameters and two nodes @ref vrpnsource to receive data from a VRPN
 *       server and @ref vrpnsink to act as a device in the server. On Windows, set the
 *       environment variable @c VRPNROOT to the vrpn directory to configure the build system.
 * @li @b USE_UBISENSE
 *      \n enables Ubisense support and the @ref ubisensesource node and @ref ubisensemodule. This works only
 *      on Windows.
 * @li @b USE_OTQT
 *      \n enables the %OTQt modules and sink nodes (see @ref page_otqt_manual). The @ref
 *      qtmouseeventmodule simulates the behaviour of the desktop mouse in 2D Qt GUI
 *      applications by deriving mouse events from tracking data. Requires Trolltech Qt
 *      library from series 3. See dedicated VS .NET 7.1 solution and project files to
 *      build OTQt. Untested on Windows!
 */

/** uncomment the following line to compile support for the ARToolKit library */
//#define USE_ARTOOLKIT 1

/** uncomment the following line to compile support for the ARToolKitPlus library */
#define USE_ARTOOLKITPLUS 1

/** uncomment the following line to compile and link against the DLL version of ARToolKitPlus */
//#define ARTOOLKITPLUS_DLL 1


/** uncomment the following line to compile ARToolKitPlusModule be a video source
 */
//#define ARTOOLKITPLUS_FOR_STB3  --- depricated !!!


/** uncomment the following line to build with an integrated OpenVideo server
    NOTE: this is not required for Studierstube 4, since Stb4 creates its oen OpenVideo instance! */
//#define USE_OPENVIDEO 1


/** We use TinyXML as standard library */
/** umcomment the following line to compile against TinyXML instead of XERCES */
#define USE_TINYXML 1

/** umcomment the following line to compile against a DLL version of TinyXML (rather than static linkage
    note: this is only required for Daniel Wagner's modified version of TinyXML
          the standard release always links statically and does not support link selection */
#define TINYXML_DLL

/** umcomment the following line to compile against a LIB version of TinyXML (rather than DLL
    note: this is only required for Daniel Wagner's modified version of TinyXML
          the standard release always links statically and does not support link selection */
//#define TINYXML_STATIC


/**
 * This define enables the correction of ARToolkit transformation matrices
 * on systems that provide camera images the right way around. ARToolkit seems
 * to expect image buffers up side down. Therefore it will compute mirrored
 * transformation matrices on normal image buffers. The correction avoids
 * the flipping of the image itself. This define must be enabled on virtually
 * any system as we do not use flipping anymore. Currently this is true only on
 * the Windows platform.
 */
#ifdef WIN32
#define ARTOOLKIT_UNFLIP_V
#endif

/** uncomment the following line to compile support for CyberMouse device.
  * Support for this is only available on windows. */
// #define USE_CYBERMOUSE 1

/** uncomment the following line to compile support for Wacom Graphire tablet.
  * Support for this is only available on windows. */
// #define USE_WACOMGRAPHIRE 1

/** uncomment the following line to compile support for Joysticks.
  * Support for this is only available on windows. */
// #define USE_JOYSTICK 1

/** uncomment the following line to compile support for 3Dconnexion SpacMouse.
  * Support for this is only available on windows. */
// #define USE_SPACEMOUSE 1

/** uncomment the following line to compile support for SAPI Speech Recognition and Text to Speech.
  * Support for this is only available on windows. Otherwise an empty implementation
  * will be used */
// #define USE_SAPISPEECH 1

/** uncomment the following line to compile support for Essentialreality's P5 glove.
  * Support for this is only available on windows yet. Linux support is in development.*/
// #define USE_P5GLOVE 1

/** uncomment the following line to compile support for the DWARF.
  */
// #define USE_DWARF 1

/** uncomment the following line to compile support for VRPN. Don't forget to set the
  * @c VRPNROOT environment variable !
  */
// #define USE_VRPN 1

#ifdef WIN32
/** uncomment the following line to compile support for the Ubisense system.
  * Support for this is only available on windows. */
// #define USE_UBISENSE 1
#endif

/** uncomment the following line to compile support for the OTQt.
  */
// #define USE_OTQT 1


