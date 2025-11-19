#!/bin/bash
# Title: File System Permission Manager
# Author: Gemini OS Lab Assistant
# Description: Checks and sets file permissions based on user input for owner, group, or others.

# Function to check current permissions
check_permissions() {
    read -p "Enter the file or directory name to check: " item_name
    
    if [ ! -e "$item_name" ]; then
        echo -e "\nERROR: '$item_name' does not exist."
        return
    fi
    
    echo -e "\n--- Current Permissions for '$item_name' ---"
    # Use ls -l to display permissions in standard format
    ls -ld "$item_name"
    
    echo -e "\n--- Detailed Analysis (Using 'test') ---"
    # Use the 'test' command (shorthand '[ ]') for specific checks
    
    # OWNER PERMISSIONS
    echo "Owner (User) Permissions:"
    [ -r "$item_name" ] && echo -e "\t- Read (r) access is granted." || echo -e "\t- Read (r) access is DENIED."
    [ -w "$item_name" ] && echo -e "\t- Write (w) access is granted." || echo -e "\t- Write (w) access is DENIED."
    [ -x "$item_name" ] && echo -e "\t- Execute (x) access is granted." || echo -e "\t- Execute (x) access is DENIED."
}

# Function to set new permissions
set_permissions() {
    read -p "Enter the file or directory name to modify: " item_name
    
    if [ ! -e "$item_name" ]; then
        echo -e "\nERROR: '$item_name' does not exist."
        return
    fi
    
    echo -e "\n--- Set Permissions ---"
    echo "Current permissions:"
    ls -ld "$item_name"
    
    # Get the new permission mode (e.g., 755, 644)
    read -p "Enter the new NUMERIC permission mode (e.g., 755, 644): " mode
    
    # Input validation (basic check for 3 digits)
    if ! [[ "$mode" =~ ^[0-7]{3,4}$ ]]; then
        echo -e "\nERROR: Invalid mode. Must be 3 or 4 octal digits (0-7)."
        return
    fi
    
    # Use the 'chmod' command to apply the new mode
    # The 'chmod' command is the core tool for setting permissions
    chmod "$mode" "$item_name"
    
    if [ $? -eq 0 ]; then
        echo -e "\nSUCCESS: Permissions set to '$mode' for '$item_name'."
        echo "New Permissions:"
        ls -ld "$item_name"
    else
        echo -e "\nERROR: Failed to change permissions. You may need 'sudo' privileges."
    fi
}

# --- Main Menu Logic ---
while true
do
    echo -e "\n--- Permission Manager Menu ---"
    echo "1. Check current permissions of an item"
    echo "2. Set new permissions (using chmod numeric mode)"
    echo "3. Exit"
    echo "-------------------------------"
    read -p "Enter your choice [1-3]: " choice
    
    case $choice in
        1) 
            check_permissions 
            ;;
        2) 
            set_permissions 
            ;;
        3) 
            echo -e "\nExiting Permission Manager. Goodbye!"
            break 
            ;;
        *) 
            echo -e "\nInvalid choice. Please enter 1, 2, or 3." 
            ;;
    esac
done
