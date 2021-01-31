#ifndef CONSTANTS_H
#define CONSTANTS_H

#define PIX_MAX 255
#define AMOLED ":/stylesheets/qss/AMOLED.qss"
#define AQUA ":/stylesheets/qss/Aqua.qss"
#define ELEGANTDARK ":/stylesheets/qss/ElegantDark.qss"
#define MATERIALDARK ":/stylesheets/qss/MaterialDark.qss"
#define NEONBUTTONS ":/stylesheets/qss/NeonButtons.qss"
#define PICPAX ":/stylesheets/qss/PicPax.qss"
#define GENETIVE ":/stylesheets/qss/Genetive.qss"
#define TCOBRA ":/stylesheets/qss/TCobra.qss"

#define MAX_HISTORY 30
#define INVERT 255
#define ZOOM_VALUE 1.05

#define LOGO_PATH ":/icon/icons/CHIMP.png"
#define ABOUT_CHIMP_TEXT \
  R"(<p>CHIMP is a an all-platform image editor created using Qt.</p> <p>The editor supports all the basic image editing tools: <ul><li>Zooming in/out of the image</li><li>Resizing the image</li><li>Cropping and selection</li><li>Applying a negative effect</li><li>Flipping the image horizontally or vertically</li><li>Rotating the image to the left or right</li><li>Freehand drawing, as well as drawing shapes such as: ellipses, rectangles and lines with a choice of colors<\li><\ul></p><p>The following sliders are also supported:<ul> <li><em>Brightness</em> increases or decreases the brightness in a uniform way across the image</li> <li><em>Contrast</em> adjusts the contrast of the image</li> <li><em>Shadows</em> adjusts the intensity of darker colors in the image</li> <li><em>Highlights</em> adjusts the intensity of lighter colors in the image</li> <li><em>Saturation</em> adjusts image saturation</li> <li><em>Correction</em> applies a colored filter of the selected color to the image</li> </ul></p> <br><br><table><thead><tr><th colspan="1">Developed by:</th></tr></thead><tbody><tr><td>Teodora Nićković      </td></tr><tr><td>Kristina Popović</td></tr><tr><td>Zoran Vujičić</td></tr><tr><td>Nemanja Tasić</td></tr><tr><td>Dunja Spasić</td></tr><tr><td>Olivera Popović</td></tr></tbody></table>)"

#define SHORTCUTS_HTML \
  R"(<table> <tr> <th colspan="3">Window Shortcuts</th> </tr> <tbody> <tr> <td>Ctrl + S</td> <td colspan="2">Save file</td> </tr> <tr> <td>Ctrl + Shift + S </td> <td colspan="2">Save file as</td> </tr> <tr> <td>Ctrl + O</td> <td colspan="2">Open file</td> </tr> <tr> <td>Ctrl + F</td> <td colspan="2">Fullscreen</td> </tr> <tr> <td>Ctrl + Q</td> <td colspan="2">Quit</td> </tr> <tr>  </tr> <tr> <th colspan="3">Editor Shortcuts</th> </tr>   <tr> <td>Ctrl + C</td> <td colspan="2">Crop</td> </tr> <tr> <td>Ctrl + R</td> <td colspan="2">Resize</td> </tr> <tr> <td>Ctrl + I</td> <td colspan="2">Invert Colors</td> </tr> </tbody> </table>)"

#endif  // CONSTANTS_H
