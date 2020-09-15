# Changelog

## v7.0.1 (under development)
*Available in the `master` branch*

### Bugfixes
- Make the Microptyhon working by adding the required variables as GC_ROOT
- Fix built-in SimSun CJK font
- Fix UTF-8 encoding when `LV_USE_ARABIC_PERSIAN_CHARS` is enabled
- Fix DMA2D usage when 32 bit images directly blended
- Fix lv_roller in infinite mode when used with encoder
- Add `lv_theme_get_color_secondary()`
- Add `LV_COLOR_MIX_ROUND_OFS` to adjust color mixing to make it compatible with the GPU
- Improve DMA2D blending
- Remove memcpy from `lv_ll` (caused issues with some optimization settings)
- `lv_chart` fix X tick drawing
- fix vertical dashed line drawing
- some additonal minor fixes and formattings

## v7.0.0 (18.05.2020)

### Documentation
The docs for v7 is available at https://docs.littlevgl.com/v7/en/html/index.html

### Legal changes

The name of the project is changed to LVGL and the new website is on https://lvgl.io

LVGL remains free under the same conditions (MIT license) and a company is created to manage LVGL and offer services.


### New drawing system
Complete rework of LVGL's draw engine to use "masks" for more advanced and higher quality graphical effects. 
A possible use-case of this system is to remove the overflowing content from the rounded edges.
It also allows drawing perfectly anti-aliased circles, lines, and arcs.
Internally, the drawings happen by defining masks (such as rounded rectangle, line, angle). 
When something is drawn the currently active masks can make some pixels transparent. 
For example, rectangle borders are drawn by using 2 rectangle masks: one mask removes the inner part and another the outer part. 

The API in this regard remained the same but some new functions were added:
- `lv_img_set_zoom`: set image object's zoom factor
- `lv_img_set_angle`: set image object's angle without using canvas
- `lv_img_set_pivot`: set the pivot point of rotation


The new drawing engine brought new drawing features too. They are highlighted in the "style" section.

### New style system
The old style system is replaced with a new more flexible and lightweighted one. 
It uses an approach similar to CSS: support cascading styles, inheriting properties and local style properties per object. 
As part of these updates, a lot of objects were reworked and the APIs have been changed. 

- more shadows options: *offset* and *spread*
- gradient stop position to shift the gradient area and horizontal gradient 
- `LV_BLEND_MODE_NORMAL/ADDITIVE/SUBTRACTIVE` blending modes
- *clip corner*: crop the content on the rounded corners
- *text underline* and *strikethrough*
- dashed vertical and horizontal lines (*dash gap*, *dash_width*)
- *outline*: a border-like part drawn out of the background. Can have spacing to the background.
- *pattern*: display and image in the middle of the background or repeat it
- *value* display a text which is stored in the style. It can be used e.g. as a lighweighted text on buttons too.
- *margin*: similar to *padding* but used to keep space outside of the object

Read the [Style](https://docs.littlevgl.com/v7/en/html/overview/style.html) section of the documentation to learn how the new styles system works.

### GPU integration
To better utilize GPUs, from this version GPU usage can be integrated into LVGL. In `lv_conf.h` any supported GPUs can be enabled with a single configuration option.

Right now, only ST's DMA2D (Chrom-ART) is integrated. More will in the upcoming releases.

### Renames
The following object types are renamed:
- sw -> switch
- ta -> textarea
- cb -> checkbox
- lmeter -> linemeter
- mbox -> msgbox
- ddlist -> dropdown
- btnm -> btnmatrix
- kb -> keyboard
- preload -> spinner
- lv_objx folder -> lv_widgets
- LV_FIT_FILL -> LV_FIT_PARENT
- LV_FIT_FLOOD -> LV_FLOOD_MAX
- LV_LAYOUT_COL_L/M/R -> LV_LAYOUT_COLUMN_LEFT/MID/RIGHT
- LV_LAYOUT_ROW_T/M/B -> LV_LAYOUT_ROW_TOP/MID/BOTTOM

### Reworked and improved object
- `dropdown`: Completely reworked. Now creates a separate list when opened and can be dropped to down/up/left/right.
- `label`: `body_draw` is removed, instead, if its style has a visible background/border/shadow etc it will be drawn. Padding really makes the object larger (not just virtually as before)
- `arc`: can draw bacground too.
- `btn`: doesn't store styles for each state because it's done naturally in the new style system.
- `calendar`: highlight the pressed datum. The used styles are changed: use `LV_CALENDAR_PART_DATE` normal for normal dates, checked for highlighted, focused for today, pressed for the being pressed. (checked+pressed, focused+pressed also work)
- `chart`: only has `LINE` and `COLUMN` types because with new styles all the others can be described. LV_CHART_PART_SERIES sets the style of the series. bg_opa > 0 draws an area in LINE mode. `LV_CHART_PART_SERIES_BG` also added to set a different style for the series area. Padding in `LV_CHART_PART_BG` makes the series area smaller, and it ensures space for axis labels/numbers.
- `linemeter`, `gauge`: can have background if the related style properties are set. Padding makes the scale/lines smaller. scale_border_width and scale_end_border_width allow to draw an arc on the outer part of the scale lines.
- `gauge`: `lv_gauge_set_needle_img` allows use image as needle
- `canvas`: allow drawing to true color alpha and alpha only canvas, add `lv_canvas_blur_hor/ver` and rename `lv_canvas_rotate` to `lv_canvas_transform`
- `textarea`: If available in the font use bullet (`U+2022`) character in text area password 

### New object types
- `lv_objmask`: masks can be added to it. The children will be masked accordingly. 

### Others
- Change the built-in fonts to [Montserrat](https://fonts.google.com/specimen/Montserrat) and add built-in fonts from 12 px to 48 px for every 2nd size.
- Add example CJK and Arabic/Persian/Hebrew built-in font
- Add ° and "bullet" to the built-in fonts
- Add Arabic/Persian script support: change the character according to its position in the text. 
- Add `playback_time` to animations.
- Add `repeat_count` to animations instead of the current "repeat forever".
- Replace `LV_LAYOUT_PRETTY` with `LV_LAYOUT_PRETTY_TOP/MID/BOTTOM`

### Demos
- [lv_examples](https://github.com/littlevgl/lv_examples) was reworked and new examples and demos were added

### New release policy
- Maintain this Changelog for every release
- Save old major version in new branches. E.g. `release/v6`
- Merge new features and fixes directly into `master` and release a patch or minor releases every 2 weeks.

### Migrating from v6 to v7
- First and foremost, create a new `lv_conf.h` based on `lv_conf_templ.h`.
- To try the new version it suggested using a simulator project and see the examples.
- If you have a running project, the most difficult part of the migration is updating to the new style system. Unfortunately, there is no better way than manually updating to the new format.
- The other parts are mainly minor renames and refactoring as described above. 
