 /* ========================================================================
  * Copyright (C) 2001  Vienna University of Technology
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the GNU Lesser General Public
  * License as published by the Free Software Foundation; either
  * version 2.1 of the License, or (at your option) any later version.
  *
  * This library is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  * Lesser General Public License for more details.
  *
  * You should have received a copy of the GNU Lesser General Public
  * License along with this library; if not, write to the Free Software
  * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  * For further information please contact Gerhard Reitmayr under
  * <reitmayr@ims.tuwien.ac.at> or write to Gerhard Reitmayr,
  * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
  * Austria.
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** header file for ARToolKit interface module.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/ARToolKitModule.h,v 1.26 2003/05/21 16:11:00 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section artoolkitmodule ARToolKitModule
 * The ARToolKitModule interfaces to the ARToolKit library to track the 
 * position and orientation of visual markers with a video camera. It uses
 * the information provided by @ref artoolkitsource nodes and inserts the state
 * events through @ref artoolkitsource nodes into the tree. The configuration 
 * element is @c ARToolKitConfig and has the following attributes :
 * @li @c camera-parameter file containing camera calibration data
 * @li @c treshhold 100 bounding value for treshholding the greyscale video
 *        image
 * @li @c framerate 10 a desired maximum framerate, the module will not exceed it.
 * @li @c videomode a string selecting the videomode for ARToolKit. This depends on 
 *        video interface linked with ARToolKit.
 * @li @c videomode2 a second string to configure a second camera for stereo mode.
 *        This is used in Studierstube to provide stereo video background.
 * @li @c pattern-dir an optional string that is prefixed to any pattern filename or
 *        the camera data filename. It tries to find the file under the original as
 *        well the prefixed name, in that order.
 *
 * See @ref artoolkitparams for possible video configuration parameter strings
 *
 * An example configuration element looks like this :
 * @verbatim
<ARToolKitConfig camera-parameter="../data/camera" treshhold="150" framerate="5" videomode="0,5,3" pattern-dir="mypatterns/"/>@endverbatim
 */

/**
 * @defgroup input Device Input Classes
 * This group contains modules and nodes implementing device drivers for
 * OpenTracker. It is expected to grow in the future to accomodate a
 * wide range of devices.
 */

/**
 * @page artoolkitparams ARToolkitModule Video Configuration
 *
 * This page describes various configuration parameters for the ARToolkit module
 * embedded in OpenTracker. These configuration strings need to be specified in
 * the @c videomode and @c videomode2 attributes of the @ref artoolkitmodule 
 * configuration element.
 *
 * @section win Windows DirectShow driver
 *
 *  parameter format is either NULL or a list of tokens, separated by commas ","
 *
 *  @subsection binary Binary Tokens
 *
 *  @li @c flipH flip image horizontally (WARNING: non-optimal performance)
 *  @li @c flipV flip image vertically (WARNING: non-optimal performance)
 *  @li @c showDlg displays either WDM capture filter's property page or
 *                 MSDV decoder format dialog (depending on source media type).
 *                 only applies to WDM_CAP, will be ignored for AVI_FILE
 *  @li @c loopAVI continuously loops through an AVI file (applies only to AVI_FILE)
 *  @li @c noClock does not use a Reference Clock to synchronize samples;
 *                 use this flag for offline post-processing (applies only to AVI_FILE)
 *  @li @c renderSec render secondary streams (applies only to AVI_FILE)
 *                   An AVI file can contain an arbitrary number of multiplexed A/V substreams,
 *                   usually there are at most 2 substreams (1st: video, 2nd: audio).
 *                   the AVI_FILE input module will only try to render stream 0x00 (assuming that
 *                   it's video) and ignore the remaning substreams.
 *                   Use this flag to force IGraphBuilder->Render(IPin*) calls on substreams 1..n
 *                   DO NOT SET this flag if your AVI file contains more than one video stream
 *
 *  @subsection param Parametrized Tokens
 *
 *   @li @c inputDevice=? supported parameters: <br>
 *                   @c "WDM_CAP"  (WDM_VIDEO_CAPTURE_FILTER) use the DirectShow WDM wrapper
 *                              to obtain live video input from a streaming capture device
 *                              such as a IEEE 1394 DV camera or USB webcam.<br>
 *                              OPTIONAL: set deviceName=? and/or ieee1394id=? for better
 *                                        control over the choice of suitable WDM drivers
 *                   @c "AVI_FILE" (ASYNC_FILE_INPUT_FILTER) use an Asynchronous File Input
 *                              Filter to read & decode AVI video data<br>
 *                              NOTE: be sure to specify the input file name by pointing
 *                                    fileName=? to a valid AVI file.<br>                  
 *                   EXAMPLE: "inputDevive=WDM_CAP", or "inputDevice=AVI_FILE"<br>
 *                   DEFAULT: "WDM_CAP" will be selected if you omit this token
 *
 *   @li @c videoWidth=? preferred video width, EXAMPLE: "videoWidth=720"
 *                   only applies to WDM_CAP, will be ignored for AVI_FILE
 *   @li @c videoHeight=? preferred video height, EXAMPLE: "videoHeight=576"
 *                   only applies to WDM_CAP, will be ignored for AVI_FILE
 *
 *   @li @c pixelFormat=? internal pixel format (see PixelFormat.h for supported types)<br>
 *                   PERFORMANCE WARNING: Be sure to match your IDirect3DTexture/OpenGL texture
 *                                        formats to whatever you specify here, i.e. use
 *                                        PXtoOpenGL(format), PXtoD3D(format) for creating your
 *                                        textures! (see PixelFormat.h for details)<br>
 *                   EXAMPLE: "pixelFormat=PIXELFORMAT_RGB32"<br>
 *					 NOTE: if you omit the pixelFormat=? token, the global
 *                         constant (default_PIXELFORMAT, usually PIXELFORMAT_RGB32)
 *                         will be selected.
 *
 *   @li @c deviceName=?  only applies to WDM_CAP, will be IGNORED if "inputDevice=WDM_CAP" is not set.
 *                   preferred WDM device (WARNING: WDM "friendly names" are locale-dependent), 
 *                   i.e. try to match substring <?> with "friendly names" of
 *                   enumerated DirectShow WDM wrappers (ksproxy.ax).<br>
 *                   EXAMPLE: "deviceName=Microsoft DV Camera" for IEEE1394 DV devices
 *                            "deviceName=QuickCam" for Logitech QuickCam
 *
 *   @li @c fileName=?  only applies to AVI_FILE, will be IGNORED if "inputDevice=AVI_FILE" is not set.
 *                   input file name, if just use a file's name (without its full path), the WIN32
 *                   API fuction SearchPath() (Winbase.h) will be used to locate the file.<br>
 *                   EXAMPLE: "fileName=C:\Some Directory\Another Directory\Video.AVI"<br>
 *                            "fileName=video.AVI" (will succeed if C:\Some Directory\Another Directory\
 *                                                  is:  * the application's startup directory
 *                                                       * the current directory
 *                                                       * listed in the PATH environment variable)
 *
 *  @li @c ieee1394id=?  only applies to WDM_CAP, will be IGNORED if "inputDevice=WDM_CAP" is not set.
 *                   Unique 64-bit device identifier, as defined by IEEE 1394.
 *					 Hexadecimal value expected, i.e. "ieee1394id=437d3b0201460008"
 *                   Use /bin/IEEE394_id.exe to determine your camera's ID.
 *
 * @li @c deinterlaceState=? supported parameters (see VFX_STATE_names[])<br>
 *                           "off"  : disable deinterlacing (DEFAULT)<br>
 *                           "on"   : force deinterlacing (even for progressive frames)<br>
 *                           "auto" : enable deinterlacing only if
 *                                    (VIDEOINFOHEADER.dwInterlaceFlags & AMINTERLACE_IsInterlaced)<br>
 *                         WARNING: EXPERIMENTAL FEATURE!
 *
 * @li @c deinterlaceMethod=? deinterlacing method (see VFxDeinterlaceParam.h for supported modes)
 *                         supported parameters (see DEINTERLACE_METHOD_names[]):<br>
 *                           "blend"      : blend fields (best quality)<br>
 *                           "duplicate1" : duplicate first field<br>
 *                           "duplicate2" : duplicate second field<br>
 *                         NOTE: omitting this token results in default mode (DEINTERLACE_BLEND) being used.<br>
 *                         WARNING: EXPERIMENTAL FEATURE!
 *
 *  @subsection examples Examples
 *@verbatim
   videomode="WDM_CAP,showDlg"
   videomode="WDM_CAP,flipH,flipV,showDlg"
   videomode="WDM_CAP,pixelFormat=PIXELFORMAT_RGB24,showDlg"
   videomode="WDM_CAP,showDlg,deinterlaceState=on,deinterlaceMethod=duplicate1"
   videomode="WDM_CAP,videoWidth=640,flipH,videoHeight=480,showDlg,deinterlaceState=auto"
   videomode="WDM_CAP,deviceName=Microsoft DV Camera,videoWidth=720,videoHeight=480"
   videomode="WDM_CAP,deviceName=Logitech,videoWidth=320,videoHeight=240,flipV"
   videomode="WDM_CAP,deviceName=Microsoft DV Camera,ieee1394id=437d3b0201460008"
   videomode="AVI_FILE,fileName=C:\Some Directory\Another Directory\Video.AVI"
   videomode="AVI_FILE,fileName=Video.AVI,pixelFormat=PIXELFORMAT_RGB24"
@endverbatim
 *
 *****************************************************************************/

#ifndef _ARTOOLKITMODULE_H
#define _ARTOOLKITMODULE_H

#include "../OpenTracker.h"

#ifdef USE_ARTOOLKIT

#ifdef WIN32
#ifndef __MEMORY_BUFFER_HANDLE__
#define __MEMORY_BUFFER_HANDLE__

	struct MemoryBufferHandle
	{
		unsigned long  n; // sample number
		__int64 t;		  // timestamp
	};

#endif
#else
    typedef int MemoryBufferHandle;
#endif

/**
 * A video tracking source module using the ARToolKit library to track a 
 * number of markers in a video image. It sets up the video library and acts
 * as a NodeFactory for ARToolKitSource nodes. It also keeps a vetor of 
 * the created nodes to update them. It is implemented as a threaded module
 * doing the grabbing and video processing in a separate thread.
 *
 * Moreover it provides a programmatic interface to get the current video image.
 * It returns information about the used format and size and a pointer to
 * the image data.
 * @ingroup input
 */
class OPENTRACKER_API ARToolKitModule : public ThreadModule, public NodeFactory
{
// members
protected:
    /// stores the sources
    NodeVector sources;
    int did;
    /// treshhold value to use in image processing
    int treshhold;
    /// flag to stop image processing thread
    int stop;
    /// file name of cameradata file
    std::string cameradata;
    /// desired maximal framerate
    double rate;
    /// videomode string
    std::string videomode;
    /// videomode string for second camera
    std::string videomode2;
    /// flag to denote if two cameras are configured
    bool stereo;

    /// pointer to the buffer map
    unsigned char * frame;
    /// size of the image in pixels
    int sizeX, sizeY;
	/// an optional prefix for pattern filenames
	std::string patternDirectory;

// methods
protected:
    /** the work method for the module thread. This method grabs a frame
     * and computes any marker information from it. Then it updates the
     * source nodes accordingly. If the stop flag is set, it finishes.*/
    virtual void run(); 

    /**
     * grabs a frame and processes the data */
    void grab();

public:
    /// constructor method
    ARToolKitModule();

    /// destructor method
    virtual ~ARToolKitModule();

    /** This method is called to construct a new Node. It compares
     * name to the ARToolKitSource element name, and if it matches
     * creates a new ARToolKitSource node.
     * @param name reference to string containing element name
     * @attributes refenrence to StringMap containing attribute values
     * @return pointer to new Node or NULL. The new Node must be
     *         allocated with new ! */
    virtual Node * createNode( const std::string& name, StringTable& attributes);

    /**
     * This method is called after initialisation is finished and before the
     * main loop is started. It opens the artoolkit library and video system.*/
    virtual void start();

    /** closes the artoolkit library */
    virtual void close();

    /**
     * pushes events into the tracker tree. Checks all source nodes for
     * new states and pushes them into the tracker tree.
     */
    virtual void pushState();

    /**
     * initializes the ARToolKit module. 
     * @param attributes StringMap of elements attribute values. Should be
     *        possibly , but is not for convenience.
     * @param localTree pointer to root of configuration nodes tree
     */
    virtual void init(StringTable& attributes, ConfigNode * localTree);

    /** returns the width of the grabbed image in pixels */
    int getSizeX();

    /** returns the height of the grabbed image in pixels */
    int getSizeY();

    /** returns whether the grabbed image is flipped horizontally
	  * or vertically */
    void getFlipping(bool* isFlippedH, bool* isFlippedV);

    /** returns a pointer to the grabbed image. The image format
     * is depending on the pixel format and typically RGB or RGBA 
     * times X times Y bytes. 
     * @return pointer to image buffer */
    unsigned char * lockFrame(MemoryBufferHandle* pHandle);
	// formerly getFrame()

    /** releases the pointer to the grabbed image.
     * @release frame pointer */
    void unlockFrame(MemoryBufferHandle Handle);

    /** 
     * returns the OpenGL flag that is used by ARToolkit to describe
     * the pixel format in the frame buffer. This is a simple way to 
     * pass the necessary information to use the image data in GL calls. */
    int getImageFormat();
};

#endif

#endif
