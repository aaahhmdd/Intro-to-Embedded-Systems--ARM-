import folium
import os

# Initialize empty list to store locations
locations = []

# Read data from file and store in locations array
with open('file.txt', 'r') as file:
    for line in file:
        lat, lon = map(float, line.strip().split(','))
        locations.append([lat, lon])

# Create a map centered at the first point
mymap = folium.Map(location=locations[0], zoom_start=4)

# Add markers for each point
for point in locations:
    folium.Marker(location=point).add_to(mymap)

# Draw a line passing through the given points
folium.PolyLine(locations=locations, color='blue').add_to(mymap)

# Specify the path to save the HTML file
html_file_path = 'map_2_with_line.html'

try:
    # Save the map as an HTML file
    mymap.save(html_file_path)
    print("Map2 saved successfully as:", os.path.abspath(html_file_path))
except Exception as e:
    print("Error occurred while saving the map2:", e)

# Display the map
mymap

