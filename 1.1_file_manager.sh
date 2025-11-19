#!/bin/bash

# Title: Simple File and Directory Management Script
# Description: Provides a menu to perform basic file and directory operations.

# Function to display the menu
show_menu() {
    echo -e "\n--- File & Directory Manager Menu ---"
    echo "1. List contents of current directory (ls -l)"
    echo "2. Check if a file/directory exists"
    echo "3. Create a new directory"
    echo "4. Create an empty file (touch)"
    echo "5. View file content (cat)"
    echo "6. Copy a file"
    echo "7. Delete a file or directory"
    echo "8. Exit"
    echo "-----------------------------------"
    read -p "Enter your choice [1-8]: " choice
}

# --- Operation Functions ---

# Option 2: Check existence of a file or directory
check_existence() {
    read -p "Enter file or directory name to check: " item_name
    
    # -e checks if the file/directory exists
    if [ -e "$item_name" ]; then
        echo -e "\nSUCCESS: '$item_name' exists."
        
        # -f checks if it is a regular file
        if [ -f "$item_name" ]; then
            echo "Type: Regular File"
        # -d checks if it is a directory
        elif [ -d "$item_name" ]; then
            echo "Type: Directory"
        else
            echo "Type: Other (e.g., pipe, device file)"
        fi
    else
        echo -e "\nERROR: '$item_name' does NOT exist."
    fi
}

# Option 3: Create a new directory
create_directory() {
    read -p "Enter name for the new directory: " dir_name
    
    # Check if directory already exists
    if [ -d "$dir_name" ]; then
        echo -e "\nERROR: Directory '$dir_name' already exists."
    else
        # Use mkdir to create the directory
        mkdir "$dir_name"
        if [ $? -eq 0 ]; then # Check exit status: 0 means success
            echo -e "\nSUCCESS: Directory '$dir_name' created."
        else
            echo -e "\nERROR: Failed to create directory '$dir_name'."
        fi
    fi
}

# Option 4: Create an empty file (touch)
create_file() {
    read -p "Enter name for the new file: " file_name
    
    # Check if file already exists
    if [ -e "$file_name" ]; then
        echo -e "\nWARNING: File '$file_name' already exists. Its timestamp will be updated."
    fi
    
    # Use touch to create an empty file or update timestamp
    touch "$file_name"
    if [ $? -eq 0 ]; then
        echo "SUCCESS: File '$file_name' created/updated."
    else
        echo "ERROR: Failed to create file '$file_name'."
    fi
}

# Option 5: View file content (cat)
view_file() {
    read -p "Enter name of the file to view: " file_name
    
    # Check if the item exists AND is a regular file
    if [ -f "$file_name" ]; then
        echo -e "\n--- Contents of '$file_name' ---"
        cat "$file_name"
        echo -e "\n------------------------------------"
    else
        echo -e "\nERROR: '$file_name' is not a regular file or does not exist."
    fi
}

# Option 6: Copy a file
copy_file() {
    read -p "Enter source file name: " src_file
    read -p "Enter destination path/file name: " dest_path
    
    # Check if source file exists
    if [ -f "$src_file" ]; then
        # Use cp command
        cp "$src_file" "$dest_path"
        if [ $? -eq 0 ]; then
            echo -e "\nSUCCESS: File '$src_file' copied to '$dest_path'."
        else
            echo -e "\nERROR: Failed to copy file. Check destination path/permissions."
        fi
    else
        echo -e "\nERROR: Source file '$src_file' does not exist."
    fi
}

# Option 7: Delete a file or directory
delete_item() {
    read -p "Enter file or directory name to delete: " item_to_delete
    
    if [ -e "$item_to_delete" ]; then
        read -p "Are you sure you want to DELETE '$item_to_delete'? (y/n): " confirm
        
        if [[ "$confirm" == "y" || "$confirm" == "Y" ]]; then
            # Use rm for files, or rm -r for directories
            if [ -d "$item_to_delete" ]; then
                # Deleting a directory, use -r (recursive)
                rm -r "$item_to_delete"
            else
                # Deleting a file
                rm "$item_to_delete"
            fi
            
            if [ $? -eq 0 ]; then
                echo -e "\nSUCCESS: '$item_to_delete' deleted."
            else
                echo -e "\nERROR: Failed to delete '$item_to_delete'. Check permissions."
            fi
        else
            echo -e "\nDeletion cancelled."
        fi
    else
        echo -e "\nERROR: '$item_to_delete' does not exist."
    fi
}

# --- Main Logic Loop ---

while true
do
    show_menu
    case $choice in
        1) 
            echo -e "\n--- Current Directory Listing ---"
            ls -l 
            ;;
        2) 
            check_existence 
            ;;
        3) 
            create_directory 
            ;;
        4) 
            create_file 
            ;;
        5) 
            view_file 
            ;;
        6)
            copy_file
            ;;
        7) 
            delete_item 
            ;;
        8) 
            echo -e "\nExiting File and Directory Manager. Goodbye!"
            break 
            ;;
        *) 
            echo -e "\nInvalid choice. Please enter a number between 1 and 8." 
            ;;
    esac
done
