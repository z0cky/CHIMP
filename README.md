# ℂℍ𝕀𝕄ℙ

Chimp is an image editor written in C++ and created using the [Qt](https://www.qt.io/) framework. This makes **Chimp** a portable tool, that can be used on either Linux, Windows or Mac OS.

**Contents of this README:**
* [Running CHIMP](#install)
* [Editor Layout](#layout)
* [Supported Sliders](#sliders)
* [Supported Tools](#tools)
* [Demo video](#video)

<a name ="install"></a>
### Running CHIMP
- Install the QT library
- Clone the repo
- Run build.sh inside the repo 
- Run the executable CHIMP file

<a name="layout"></a>
### Editor Layout
![Editor Layout](./resource/screenshots/editor-layout.png)

The explanation for each of the highlighted areas is as follows:
- The <span style="color:green">green</span> area represents the menu bar with the usual selection of options:
  - Opening and saving image files
  - Changing the Theme of the editor
  - Undoing/redoing changes (the usual Ctrl+Z and Ctrl+Shift+Z shortcuts can be used)
  - Getting a list of supported shortcuts
- The <span style="color:red">red</span> area contains non-slider tools available. For a full list of supported tools, read more [here](#tools)
- The <span style="color:yellow">yellow</span> area contains histograms, either grayscale or RGB depending on which histogram has been selected. In the image editing context a color histogram is a representation of the distribution of colors.
- The <span style="color:magenta">magenta</span> area contains adjustable sliders and color correction. For a full list of supported sliders, visit [this section](#sliders).


<a name="sliders"></a>
### Supported Sliders

- *Brightness* increases or decreases the brightness in a uniform way across the image
- *Contrast* adjusts the contrast of the image
- *Shadows* adjusts the intensity of darker colors in the image
- *Highlights* adjusts the intensity of lighter colors in the image
- *Saturation* adjusts image saturation
- *Correction* applies a colored filter of the selected color to the image

<a name="tools"></a>
### Supported Tools

The editor supports the following editing tools:
<pre>
 <img src="./resource/icons/001-picture.png" alt="Open" width="25" align="left"/>➤ Open the image

 <img src="./resource/icons/undo.png" alt="Undo" width="25" align="left"/>➤ Undo

 <img src="./resource/icons/redo.png" alt="Redo" width="25" align="left"/>➤ Redo

 <img src="./resource/icons/selection.png" alt="Selection" width="25" align="left"/>➤ Select the image area

 <img src="./resource/icons/034-crop.png" alt="Crop" width="25" align="left"/>➤ Crop the current image

 <img src="./resource/icons/011-grid.png" alt="Grid" width="25" align="left"/>➤ Resize the current image

 <img src="./resource/icons/029-flip2.png" alt="Flip horizontally" width="25" align="left"/>➤ Flip the current image horizontally

 <img src="./resource/icons/029-flip.png" alt="Flip vertically" width="25" align="left"/>➤ Flip the current image vertically

 <img src="./resource/icons/rotation2.png" alt="Rotate to the left" width="25" align="left"/>➤ Rotate the current image to the left

 <img src="./resource/icons/030-rotation.png" alt="Rotate to the right" width="25" align="left"/>➤ Rotate the current image to the right

 <img src="./resource/icons/invert.png" alt="Invert" width="25" align="left"/>➤ Apply a invert color effect to the current image

 <img src="./resource/icons/width.png" alt="Width" width="25" align="left"/>➤ Brush style (width, opacity, color, square, gradient)

 <img src="./resource/icons/paintbrush.png" alt="Brush" width="25" align="left"/>➤ Freehand drawing on the current image

 <img src="./resource/icons/elipse.png" alt="Elipse" width="25" align="left"/>➤ Drawing ellipses

 <img src="./resource/icons/rectangle.png" alt="Rectangle" width="25" align="left"/>➤ Drawing rectangles

 <img src="./resource/icons/line.png" alt="Line" width="25" align="left"/>➤ Drawing straight lines

 <img src="./resource/icons/041-colors.png" alt="Picker" width="25" align="left"/>➤ Choose freehand or shape drawing color

</pre>

<a name ="video"></a>
### Demo video <br>
![](video/demo.mp4)


## Developers

- [Teodora Nickovic, 57/2016](https://gitlab.com/art3mida)
- [Kristina Popovic, 58/2016](https://gitlab.com/FriendlyBytes)
- [Zoran Vujicic, 158/2015](https://gitlab.com/z0cky)
- [Nemanja Tasic, 250/2014](https://gitlab.com/tasic92)
- [Dunja Spasic, 73/2016](https://gitlab.com/dunjx)
- [Olivera Popovic, 126/2016](https://gitlab.com/OliveraPopovic)
