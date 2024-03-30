# Hover Racer
## Functionality
The hover moves using the WASD keys. When the player presses W, the hover moves forward at a constant speed, which increases if the key is held down. When the player presses S, the hover's speed decreases or it starts moving backwards. The A and D keys are used for rotating the hover left and right.
When the S and A keys are released, the hover's speed gradually decreases due to friction. There is no upper limit to the speed of the hover. If the hover collides with an object such as a wall, area boundary, water tank, or checkpoint leg, it will be thrown back. The hover must pass through the checkpoints in the order they are placed on the track. When the hover passes under the checkpoint arch, the game moves to the next stage, where the hover should go through the next checkpoint. The game is over when all 11 checkpoints on the track have been completed in the correct order.
The hover can also be boosted for up to 4 seconds at a time. During a boost, the hover accelerates at twice its normal speed. If the boost is used for longer than 4 seconds, the boosting function becomes unavailable for a period.
There are 3 speed boost packages that appear on the track. They look like small flames. When the hover runs over a speed boost package, its speed doubles for 2 seconds. The player cannot use more than one speed boost package at the same time.
The player can change the camera view by pressing the 1-4 keys. Each camera can be moved using the arrow keys. The C key resets the camera to its default position.
## Implementation 
### Collision detection
The game features two types of collision detection: box-to-sphere (for the hover and box objects) and sphere-to-sphere (for the hover and checkpoints or other objects represented by spheres). All objects are divided into two groups and stored in separate arrays. First, all box objects are checked for collision with the hover in a for-loop, and then all objects represented by spheres are checked.
Since all objects are in the same plane perpendicular to the OY axis, the Y dimension can be ignored for collision detection in two-dimensional space.
#### Box to sphere collision detection function
To implement this function, you need to find the point of the rectangle closest to the circle. To do this, you need to project a vector connecting the centers of the circle and the rectangle onto one of the sides of the rectangle. Postponing these projections from the center of the rectangle, you can find the nearest point of the rectangle to the circle. Now it remains to compare the distance from this point to the center of the circle with the radius of this circle. The distance is found by the formula following from the Pythagorean theorem. You can compare squared values so as not to waste time extracting roots.
#### Sphere to sphere collision detection
This function is much simpler than the previous one, since here you just need to compare the distance between the centers of the circles with the sum of the radii, which is also calculated using the Pythagorean theorem.
#### Speed after collision is detected
When a collision is detected, it means that the hover has crashed into some object. Hover must bounce off the object, and, according to the law of conservation of momentum, its speed must be preserved, it will simply be directed in the opposite direction.
