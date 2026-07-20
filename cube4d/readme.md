# Project Workflow

This is the flow of the program, starting in `main` and following the functions in the order they are used.

## 1. Starting the program

### `main`

`main` checks that a map path was provided, starts MiniLibX, loads the wall texture, and creates the window and drawing image. It then prepares the player, keyboard state, timer, and map before registering the input hooks and handing control to the MiniLibX loop.

### `load_image`

`load_image` reads the XPM wall texture into a MiniLibX image and stores its size. It also gets the image's raw pixel address so the renderer can later copy individual texture colours onto the screen.

### `init_player`

`init_player` places the player at `(100, 100)`. The direction values are filled in later by the game loop from the player's angle.

### `init_keys`

`init_keys` marks W, A, S, and D as not pressed. This gives the game loop a clean keyboard state when the program begins.

### `get_time_ms`

`get_time_ms` returns the current time in milliseconds. The program compares this value between frames so movement and rotation stay consistent even when the frame rate changes.


## 3. Connecting input and starting the loop

### `key_input`

`key_input` connects key presses to `key_press`, key releases to `key_release`, and every loop cycle to `game_loop`. Once `main` calls `mlx_loop`, MiniLibX repeatedly triggers these registered functions.

### `key_press`

`key_press` sets the matching W, A, S, or D flag while a key is held down. Pressing Escape destroys the window and exits the program.

### `key_release`

`key_release` clears the matching movement flag when W, A, S, or D is released. This stops the related movement or rotation on the next frame.

## 4. Updating each frame

### `game_loop`

`game_loop` calculates the time since the previous frame and uses it to move forward or backward with W and S, or rotate with A and D. It checks proposed movement with `hit_wall`, updates the player's direction, and then asks `render_scene` to draw the new frame.

### `hit_wall`

`hit_wall` converts a world position into map-grid coordinates by dividing it by `TILE_SIZE`. Positions outside the map count as walls, while positions inside collide only when their map character is `'1'`.

## 5. Drawing the scene

### `render_scene`

`render_scene` clears the old frame, draws the top-down map and player, and then casts the rays that build the 3D view. When the image is complete, it places the finished frame in the window.

### `clear_image`

`clear_image` visits every pixel in the frame image and paints it black. This removes the previous frame before anything new is drawn.

### `draw_map`

`draw_map` walks through every character in the loaded map. It draws wall tiles (`'1'`) in dark grey and open tiles (`'0'`) in white on the left side of the image.

### `draw_square`

`draw_square` fills one `TILE_SIZE` by `TILE_SIZE` area with a single colour. It is the small building block used to turn each map character into a visible tile.

### `put_pixel`

`put_pixel` calculates the memory location of one pixel and writes the requested colour there. It ignores coordinates outside the window so an invalid draw does not write beyond the image buffer.

### `draw_player`

`draw_player` marks the player's position with a small red square on the top-down map. It also draws a short green line based on the player's angle to show which way they are facing.

## 6. Casting rays

### `cast_all_rays`

`cast_all_rays` uses the space to the right of the top-down map as the width of the 3D view and sends one ray through each screen column. For every ray, it corrects the distance to reduce the fish-eye effect, then draws that column's wall, ceiling, and floor.

### `ray_caster`

`ray_caster` starts a ray at the player's position and moves it forward one small step at a time using the ray angle. When it reaches a wall tile, it records the wall side that was hit and calculates the distance from the player to the hit point.

### `calculate_direction`

`calculate_direction` compares the ray's old map cell with its new one when it enters a wall. From that change, it records whether the ray hit the north, south, east, or west face of the wall.

## 7. Building the 3D view

### `draw_walls`

`draw_walls` turns the ray distance into the height and vertical position of one wall slice on screen. It chooses the correct texture orientation for the wall face, samples the texture row by row, and draws the slice into its screen column.

### `find_wall_x`

`find_wall_x` finds the ray's horizontal position inside the wall tile it hit. North- and south-facing walls use this offset to choose the texture column.

### `find_wall_y`

`find_wall_y` finds the ray's vertical position inside the wall tile it hit. East- and west-facing walls use this offset to choose the texture column.

### `get_pixel`

`get_pixel` reads one colour directly from the wall texture's image memory. It returns black if the requested texture coordinates are outside the image.

### `draw_roof`

`draw_roof` fills the part of a screen column above the wall slice with the ceiling colour. The wall's calculated height tells it where to stop.

### `draw_floor`

`draw_floor` fills the part of a screen column below the wall slice with the floor colour. It starts at the bottom of the wall and continues to the bottom of the window.

---















# The Maths Behind My Project

You do not need to be amazing at maths to understand how my raycaster works. Most of the maths is just positions, directions, distances and converting the map into pixels that can be drawn on the screen.

## Why I use time in the game

The game loop will not always run at the exact same speed.

A powerful computer might run the loop more times every second, while a slower computer might run it fewer times.

`get_time_ms()` gets the current time in milliseconds.

I subtract `last_time` from the current time to find out how long the previous frame took.

I then divide by `1000` to convert the time from milliseconds into seconds.

For example, if the previous frame took 20 milliseconds:

```text
20 / 1000 = 0.02 seconds
```

I use this time when moving the player.

If the player moves at 80 pixels per second and the frame took 0.02 seconds:

```text
80 * 0.02 = 1.6 pixels
```

The player moves 1.6 pixels during that frame.

This means the player's movement stays around the same speed even when the game runs on different computers.

## The player's position

The player has an `x` position and a `y` position.

I think of the screen like a graph:

```text
(0, 0) --------------------> x
  |
  |
  |
  v
  y
```

Increasing `x` moves the player to the right.

Increasing `y` moves the player down.

My map is made from square tiles that are 64 pixels wide and 64 pixels high.

If the player is at:

```text
x = 100
y = 100
```

I can find the player's map position by dividing both values by `TILE_SIZE`.

```text
100 / 64 = 1
```

The player is therefore inside map column `1` and map row `1`.

The decimal part is ignored because map coordinates use whole numbers.

## What the player's angle means

`player_ang` stores the direction the player is facing.

The angle is stored in radians instead of degrees.

Degrees and radians both measure angles. They are just two different units, like metres and feet.

A full circle is 360°
A full circle is also 2π radians
Half a circle is 180° or π radians
A quarter circle is 90° or π / 2 radians

A complete turn is:

```text
2 * PI radians
```

This is the same as 360 degrees.

```text
0 radians        = right
PI / 2 radians   = down
PI radians       = left
3 * PI / 2       = up
2 * PI           = back to right
```
**How the player turns**

The player's direction is stored inside player_ang.

When I press A, I reduce the angle:

player_ang -= rot_speed * delta_time;

When I press D, I increase the angle:

player_ang += rot_speed * delta_time;

Changing the angle makes the player face a different direction.

What rot_speed means

rot_speed is the player's rotation speed.

It decides how quickly the player turns, usually measured in radians per second.

For example, if I set:

player->rot_speed = 2.0;

this means the player can rotate by about 2 radians every second.

A bigger value makes the player turn faster:

player->rot_speed = 4.0;

A smaller value makes the player turn more slowly:

player->rot_speed = 1.0;

What delta_time means

delta_time stores how long the previous frame took to run.

I calculate it using the current time and the time of the previous frame:

now = get_time_ms();
delta_time = (now - app->last_time) / 1000.0;
app->last_time = now;

get_time_ms() gives the time in milliseconds.

I subtract last_time from now to find out how many milliseconds passed since the previous frame.

I divide by 1000.0 to convert milliseconds into seconds.

For example, if the previous frame took 20 milliseconds:

20 / 1000 = 0.02 seconds

So:

delta_time = 0.02
Why I multiply them together

I multiply rot_speed by delta_time to find out how much the player should turn during the current frame.

rotation_amount = rot_speed * delta_time;

For example, if:

rot_speed = 2 radians per second
delta_time = 0.02 seconds

then:

rotation_amount = 2 * 0.02
rotation_amount = 0.04 radians

The player's angle changes by 0.04 radians during that frame.

This makes turning depend on real time instead of the number of frames.

Without delta_time, a computer running more frames per second would turn the player faster because the angle would be changed more often.

Using delta_time keeps the turning speed roughly the same on both fast and slow computers.

## Turning an angle into a direction

The angle tells me where the player is facing, but movement needs an amount for both the x-axis and the y-axis.

I use `cos()` and `sin()` to turn the angle into an x direction and a y direction.

## Turning the player's angle into movement

The player's angle tells me which direction the player is facing, but an angle by itself cannot directly change the player's `x` and `y` position.

To move the player, I need to work out two things:

```text
how much to move across the x-axis
how much to move across the y-axis
```

I use `cos()` and `sin()` to split the player's angle into those two parts:

```c
direction_x = cos(player_ang);
direction_y = sin(player_ang);
```

`cos(player_ang)` gives me the horizontal part of the direction.

`sin(player_ang)` gives me the vertical part of the direction.

Together, they create a direction vector:

```text
(direction_x, direction_y)
```

This vector tells me which way the player is facing.

### Facing right

If the player's angle is `0` radians:

```text
cos(0) = 1
sin(0) = 0
```

The direction becomes:

```text
(1, 0)
```

This means:

```text
move positively on the x-axis
do not move on the y-axis
```

So the player moves directly to the right.

### Facing down

If the player's angle is `PI / 2` radians, which is 90 degrees:

```text
cos(PI / 2) ≈ 0
sin(PI / 2) = 1
```

The direction becomes:

```text
(0, 1)
```

This means:

```text
do not move on the x-axis
move positively on the y-axis
```

Because screen coordinates increase downwards, this moves the player down.

### Facing diagonally

The direction does not always have to be completely horizontal or vertical.

For example, at 45 degrees:

```text
player_ang = PI / 4
```

The values are roughly:

```text
cos(PI / 4) = 0.707
sin(PI / 4) = 0.707
```

The direction becomes:

```text
(0.707, 0.707)
```

This means the player moves partly right and partly down at the same time.

That creates diagonal movement.

### Using `move_step`

The direction vector only tells me the direction. It does not decide how far the player should move.

`move_step` stores the distance the player should travel during the current frame.

I calculate the next position by multiplying the direction by `move_step`:

```c
next_x = player_x + cos(player_ang) * move_step;
next_y = player_y + sin(player_ang) * move_step;
```

For example, if the player is facing right:

```text
cos(player_ang) = 1
sin(player_ang) = 0
move_step = 2
```

The calculation becomes:

```text
next_x = player_x + 1 * 2
next_y = player_y + 0 * 2
```

So the player moves 2 pixels to the right and does not move vertically.

If the player is facing diagonally at 45 degrees:

```text
cos(player_ang) = 0.707
sin(player_ang) = 0.707
move_step = 2
```

The movement becomes roughly:

```text
x movement = 0.707 * 2 = 1.414
y movement = 0.707 * 2 = 1.414
```

So the player moves about 1.4 pixels right and 1.4 pixels down.

### Moving backwards

To move forward, I add the direction:

```c
next_x = player_x + cos(player_ang) * move_step;
next_y = player_y + sin(player_ang) * move_step;
```

To move backwards, I subtract it:

```c
next_x = player_x - cos(player_ang) * move_step;
next_y = player_y - sin(player_ang) * move_step;
```

The direction values do not change. I am just applying them in the opposite direction.

For example, if the player is facing right:

```text
direction = (1, 0)
```

Moving forward adds `(1, 0)`, so the player moves right.

Moving backwards subtracts `(1, 0)`, which is the same as adding `(-1, 0)`, so the player moves left.

The player is still facing right, but is walking backwards.


## How wall collision works

The player's position is measured in pixels, but the map is stored using rows and columns.

I divide the pixel position by `TILE_SIZE` to convert it into a map position.

```c
map_x = x / TILE_SIZE;
map_y = y / TILE_SIZE;
```

For example:

```text
x = 150
y = 90
```

With tiles that are 64 pixels wide:

```text
150 / 64 = 2
90 / 64 = 1
```

The player is inside map cell:

```text
map_visual[1][2]
```

If that position contains `'1'`, it means there is a wall there.

Before changing the player's real position, I check `next_x` and `next_y`.

The code is basically asking:

> If the player takes this next step, will they enter a wall?

If the answer is yes, the movement is rejected.

If the answer is no, the player's position is updated.

## The field of view

The field of view, or FOV, is the total amount of the game world that the player can see at one time.

My project uses:

```text
1.047 radians
```

This is roughly 60 degrees.

The player is facing one main direction, stored in `player_angle`, but I do not cast every ray at exactly that same angle.

Instead, I spread the rays across the full 60-degree field of view.

The first ray starts half of the FOV to the left of the player’s direction:

```c
player_angle - (FOV / 2)
```

The last ray finishes half of the FOV to the right:

```c
player_angle + (FOV / 2)
```

This calculation finds the angle for each ray:

```c
ray_angle = player_angle - (FOV / 2)
	+ ((double)x / view_width) * FOV;
```

`x` represents the current screen column.

When `x` is near `0`, the ray is near the left side of the FOV.

When `x` is near the middle of the screen, the ray is close to `player_angle`.

When `x` is near the end of the screen, the ray is near the right side of the FOV.

For example, if the player is facing 90 degrees and the FOV is 60 degrees, the rays cover roughly:

```text
60 degrees to 120 degrees
```

The centre ray is around 90 degrees, which is the direction the player is directly looking.

Each ray represents one thin vertical column on the screen.

The leftmost ray creates the leftmost column, the centre ray creates the centre column, and the rightmost ray creates the rightmost column.

By repeating this for every screen column, I build the full 3D view.

## How a ray gets its direction

Once I know the angle of one ray, I use `cos()` and `sin()` to turn that angle into movement values:

```c
ray_dx = cos(ray_angle);
ray_dy = sin(ray_angle);
```

These values are not the ray’s position.

They tell me how much the ray should move on each axis during one step.

`ray_dx` tells me how much to move across the x-axis.

`ray_dy` tells me how much to move across the y-axis.

For example, if the ray angle is `0` radians:

```text
cos(0) = 1
sin(0) = 0
```

The direction becomes:

```text
ray_dx = 1
ray_dy = 0
```

This means the ray moves one step to the right and does not move vertically.

If the ray angle is 45 degrees:

```text
ray_dx ≈ 0.707
ray_dy ≈ 0.707
```

This means the ray moves partly right and partly down on every step.

The values from `cos()` and `sin()` can be anywhere between `-1` and `1`.

A positive x value moves the ray right.

A negative x value moves it left.

A positive y value moves it down.

A negative y value moves it up.

## How the ray travels

Every ray begins at the player’s current position:

```c
ray_x = player_x;
ray_y = player_y;
```

So `ray_x` and `ray_y` are the ray’s current coordinates.

The direction values are then added to that position:

```c
ray_x += ray_dx;
ray_y += ray_dy;
```

This means:

```text
new position = current position + direction step
```

For example, if the ray starts at:

```text
ray position = (100, 100)
```

and its direction is:

```text
ray_dx = 1
ray_dy = 0
```

then the positions become:

```text
(100, 100)
(101, 100)
(102, 100)
(103, 100)
```

The direction remains `(1, 0)`, but the position changes every time it is added.

For a diagonal ray with:

```text
ray_dx = 0.707
ray_dy = 0.707
```

the positions become roughly:

```text
(100.000, 100.000)
(100.707, 100.707)
(101.414, 101.414)
(102.121, 102.121)
```

So `cos()` and `sin()` do not return the next complete coordinate.

They return the x and y step values that are used to calculate the next coordinate.

The ray keeps repeating this process:

```text
move one small step
check the current map tile
move another step
check again
```

After each step, I convert the ray’s pixel position into a map position:

```c
map_x = ray_x / TILE_SIZE;
map_y = ray_y / TILE_SIZE;
```

Then I check the map:

```c
if (map_visual[map_y][map_x] == '1')
```

If the map position contains `'1'`, the ray has entered a wall.

At that point, the loop stops and I can calculate how far the wall is from the player.

So the full process for one ray is:

```text
choose the ray angle
→ use cos and sin to get the direction step
→ start at the player’s position
→ add the direction step to the position
→ check the map
→ repeat until a wall is reached
```

## How a ray travels

Every ray begins at the player's position.

I use `cos()` and `sin()` again to find the direction of the ray.

```c
ray_dx = cos(ray_angle);
ray_dy = sin(ray_angle);
```

The ray then repeatedly adds these values to its position.

```c
ray_x += ray_dx;
ray_y += ray_dy;
```

In simple terms, the ray takes small steps in a straight line.

It keeps moving until it enters a map tile containing `'1'`.

That means the ray has hit a wall.

## How I know which side of a wall was hit

Before each ray step, I save the old map row and column.

After the ray enters a wall, I compare the old map position with the new one.

This tells me which grid boundary the ray crossed.

```text
new map_x > old map_x  -> ray moved right
new map_x < old map_x  -> ray moved left
new map_y > old map_y  -> ray moved down
new map_y < old map_y  -> ray moved up
```

For example, if the ray moves from map column `2` into map column `3`, the ray travelled to the right.

It entered the wall through the wall's left side, which can be described as the west face.

The wall direction matters because different wall faces may use different textures.

It also tells me when a texture needs to be flipped.

Without flipping the texture coordinates correctly, textures on opposite walls can appear mirrored.

There is one detail I need to check in my current `calculate_direction()` function.

If the ray moves down, it enters the wall through the wall's north face.

If the ray moves up, it enters through the wall's south face.

My current code may store these two directions the opposite way around, so I should swap them if my enum is meant to describe the side of the wall that was actually touched.

## How the ray distance is calculated

Once the ray hits a wall, I have two points:

```text
the player's position
the wall-hit position
```

These points form a right-angled triangle.

```text
horizontal difference = ray_x - player_x
vertical difference   = ray_y - player_y
```

I calculate the distance using:

```c
distance = sqrt((dx * dx) + (dy * dy));
```

This uses the Pythagorean theorem:

```text
a² + b² = c²
```

For example, if the ray moved 3 pixels horizontally and 4 pixels vertically:

```text
distance = sqrt(3² + 4²)
distance = sqrt(9 + 16)
distance = sqrt(25)
distance = 5
```

The wall is 5 pixels away from the player.

## Why fish-eye correction is needed

Rays near the left and right sides of the screen travel diagonally.

Because they travel diagonally, their raw distance is longer than the centre ray, even when every ray hits the same flat wall.

If I use these raw distances directly, a straight wall appears curved.

This is called the fish-eye effect.

I correct the distance using:

```c
corrected_distance = distance
    * cos(ray_angle - player_angle);
```

`ray_angle - player_angle` tells me how far the ray is pointing away from the centre of the player's view.

Multiplying by the cosine removes the extra diagonal distance.

This means the wall distance is measured based on how far the wall is directly in front of the camera.

As a result, flat walls appear flat instead of curved.

## How wall height is calculated

Walls close to the player should look tall.

Walls far away should look short.

I create this perspective by dividing a fixed value by the wall distance.

```c
line_height = (TILE_SIZE * 320) / distance;
```

For example, if the wall is 64 pixels away:

```text
(64 * 320) / 64 = 320 pixels
```

If the wall is twice as far away at 128 pixels:

```text
(64 * 320) / 128 = 160 pixels
```

Doubling the distance halves the wall height.

The value `320` acts like the camera projection distance.

A larger value makes the walls appear taller and more zoomed in.

A smaller value makes the walls appear shorter and more zoomed out.

## Keeping the wall in the middle of the screen

Once I know the wall height, I place it around the centre of the window.

```c
line_start = (WINDOW_HEIGHT / 2) - (line_height / 2);
line_end = (WINDOW_HEIGHT / 2) + (line_height / 2);
```

My window height is 512 pixels.

The middle of the window is therefore:

```text
512 / 2 = 256
```

If a wall is 200 pixels tall:

```text
line_start = 256 - 100 = 156
line_end = 256 + 100 = 356
```

This leaves the same amount of space above and below the wall.

Very close walls can be taller than the actual window.

Because of this, I clamp the visible drawing positions.

```c
if (draw_start < 0)
    draw_start = 0;
if (draw_end > WINDOW_HEIGHT)
    draw_end = WINDOW_HEIGHT;
```

I still keep the original `line_start` for the texture calculation.

This means the texture is cropped when the wall is too tall instead of being stretched incorrectly.

## Choosing the horizontal texture position

When the ray hits a wall, the exact hit position tells me which vertical strip of the texture should be drawn.

First, I find where the wall tile begins.

```c
tile_start = map_coordinate * TILE_SIZE;
```

I then subtract that from the ray's hit position.

```c
hit_part = ray_position - tile_start;
```

For example, if the wall tile begins at pixel `128` and the ray hits at pixel `150`:

```text
150 - 128 = 22
```

The ray hit the wall 22 pixels across the tile.

Depending on which wall face was hit, I use either the ray's x offset or y offset as the texture's horizontal coordinate.

This chooses which vertical strip of the texture appears on that screen column.

## Choosing the vertical texture position

Every screen pixel between the top and bottom of the wall needs a matching pixel from the texture.

I calculate the texture's vertical position using:

```c
texture_y = ((y - line_start) * texture_height)
    / line_height;
```

At the top of the wall, `texture_y` is near the top of the texture.

Halfway down the wall, it is near the middle of the texture.

At the bottom of the wall, it is near the bottom of the texture.

This scales the texture so it fits walls of different heights.

A close wall might be very tall, while a distant wall might be short, but the full texture can still be mapped across both of them.

## Drawing the ceiling and floor

The top of the wall separates the wall from the ceiling.

The bottom of the wall separates the wall from the floor.

The screen column is split into three sections:

```text
0 to line_start              = ceiling
line_start to line_end       = wall
line_end to WINDOW_HEIGHT    = floor
```

`draw_roof()` draws from the top of the screen down to `line_start`.

```c
y = 0;
while (y < line_start)
{
    put_pixel(&app->img, screen_x, y, ceiling_colour);
    y++;
}
```

`draw_floor()` starts at `line_end` and continues to the bottom of the window.

```c
y = line_end;
while (y < WINDOW_HEIGHT)
{
    put_pixel(&app->img, screen_x, y, floor_colour);
    y++;
}
```

The ceiling, wall and floor functions all use the same wall-height calculation.

This makes sure they agree on where the wall starts and finishes.

Together, they fill one complete vertical screen column:

```text
ceiling
wall texture
floor
```

The raycaster repeats this process for every ray and every screen column.

Once all the columns are drawn beside each other, they create the full 3D image.
