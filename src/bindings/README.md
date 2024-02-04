## Project Setup and Build Instructions

### Setting Up the Project

1. **Clone the Repository**
   - Clone this repository to your local machine.
   - Ensure you have Python, CMake, and a C++ compiler installed.

2. **Initialize Submodules**
   - If the project uses Git submodules (e.g., for `pybind11`), initialize them:
     ```
     git submodule update --init --recursive
     ```

### Building the C++ Library

1. **Navigate to the Build Directory**
   - From the project root, create a new build directory (if not already present) and navigate into it:
     ```
     mkdir build && cd build
     ```

2. **Generate Build Configuration**
   - Run CMake to configure the project:
     ```
     cmake ..
     ```

3. **Compile the Library**
   - Compile the `LedTableApiLib` library:
     ```
     cmake --build . --target LedTableApiLib
     ```

### Setting Up Python Bindings

1. **Compile Python Bindings**
   - Ensure the `LedTableApiLib` is built as per the above steps.
   - Build the Python bindings module (`ledtableapi`):
     ```
     cmake --build . --target ledtableapi
     ```

### Testing the Bindings

1. **Testing Python Bindings**
   - After building, the Python extension module will be in the build directory. Test it by running:
     ```
     python -c "import ledtableapi; ledtableapi.hello_world()"
     ```

### Example usage
```
import tc_led_table  # Import the module containing the Pybind11 bindings

# Assuming you have a function to get the API instance, similar to what was discussed
api = tc_led_table.getApiInstance({"mqtt_broker": {"host": "192.168.0.1"}})

# Set suppress messages
api.setSuppressMessages(True)  # Or False to enable messages

# Fill a single node by nodeId
node_id = 5
color = 0xFF000000  # Assuming WRGB is a uint32_t representing a color, here 0xWWRRGGBB
api.fillNode(node_id, color)

# Fill a node using a RingCoordinate
ring_coord = tc_led_table.RingCoordinate(1, 2)  # Example ring and index
api.fillNode(ring_coord, color)

# Fill a node using a Cartesian2dCoordinate
cartesian_coord = tc_led_table.Cartesian2dCoordinate(1.0, 2.0)  # Example x and y
api.fillNode(cartesian_coord, color)

# Fill a node using a CubeCoordinate
cube_coord = tc_led_table.CubeCoordinate(1, -1, 0)  # Example q, r, s coordinates
api.fillNode(cube_coord, color)

# Fill a node with a sequence of colors and a pad color
node_id = 6
colors = [0xFF0000, 0x00FF00, 0x0000FF]  # A list of colors
pad_color = 0x000000  # Black as pad color
api.fillNode(node_id, colors, pad_color)

# Set a specific pixel's color in a node
pixel_index = 3
api.setNodePixel(node_id, pixel_index, color)

# Set a pixel's color using a RingCoordinate
api.setNodePixel(ring_coord, pixel_index, color)

# Set a pixel's color using a Cartesian2dCoordinate
api.setNodePixel(cartesian_coord, pixel_index, color)

# Set a pixel's color using a CubeCoordinate
api.setNodePixel(cube_coord, pixel_index, color)

# Queue a pixel color to a node and get the dequeued color
dequeued_color = api.queueNodePixel(node_id, color)  # Returns the color removed from the queue

# Queue a pixel color using a RingCoordinate
dequeued_color = api.queueNodePixel(ring_coord, color)

# Queue a pixel color using a Cartesian2dCoordinate
dequeued_color = api.queueNodePixel(cartesian_coord, color)

# Queue a pixel color using a CubeCoordinate
dequeued_color = api.queueNodePixel(cube_coord, color)

# Dequeue a pixel color from a node
dequeued_color = api.dequeueNodePixel(node_id, color)  # Returns the color removed from the queue

# Dequeue a pixel color using a RingCoordinate
dequeued_color = api.dequeueNodePixel(ring_coord, color)

# Dequeue a pixel color using a Cartesian2dCoordinate
dequeued_color = api.dequeueNodePixel(cartesian_coord, color)

# Dequeue a pixel color using a CubeCoordinate
dequeued_color = api.dequeueNodePixel(cube_coord, color)

# Reset the LED table
api.reset()

# Refresh the state of the LED table, updating all clusters
# with the controller's buffer model
api.refresh()
```

### Notes

- Replace `LedTableApiLib` and `ledtableapi` with actual target names used in your project if they differ.
- Adjust paths and commands based on your operating system and project structure.
