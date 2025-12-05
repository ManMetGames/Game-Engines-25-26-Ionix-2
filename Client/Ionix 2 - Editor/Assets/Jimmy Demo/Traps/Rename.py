import os

# Use the current working directory
parent_folder = os.getcwd()

for root, dirs, files in os.walk(parent_folder):
    for folder in dirs:
        folder_path = os.path.join(root, folder)

        # Loop through files inside this folder
        for filename in os.listdir(folder_path):
            file_path = os.path.join(folder_path, filename)

            # Skip directories
            if os.path.isdir(file_path):
                continue

            # Split filename and extension
            name, ext = os.path.splitext(filename)

            # Build the new filename
            new_name = f"{name}_{folder}{ext}"
            new_path = os.path.join(folder_path, new_name)

            # Rename the file
            os.rename(file_path, new_path)
            print(f"Renamed: {filename} -> {new_name}")
