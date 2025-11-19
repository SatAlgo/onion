#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <iomanip>

using namespace std;

// --- 1. Disk Parameters and Constants ---
const int DISK_BLOCKS = 32;       // Total number of simulated disk blocks
const int INODE_COUNT = 8;        // Total number of available inodes
const int MAX_BLOCK_POINTERS = 4; // Max direct block pointers per inode
const int BLOCK_SIZE = 128;       // Data size per block (simulated)

// --- 2. Inode Structure (File Metadata) ---
struct Inode {
    bool allocated = false;       // True if this inode is in use
    int file_size = 0;
    string file_name = "";
    int direct_blocks[MAX_BLOCK_POINTERS] = {0}; // Pointers to disk block indices

    // Constructor to reset blocks to 0 (unallocated)
    Inode() {
        fill(begin(direct_blocks), end(direct_blocks), 0);
    }
};

// --- 3. Global File System State ---
// Map to simulate the Directory (Filename -> Inode Index)
map<string, int> directory;

// Array to simulate the Inode Table
Inode inode_table[INODE_COUNT];

// Vector to simulate the Disk Block Free List (0=Free, 1=Used)
vector<int> disk_free_list(DISK_BLOCKS, 0); 

// --- Core File System Functions ---

/**
 * @brief Allocates a free disk block from the free list.
 * @return The index of the allocated block, or -1 if the disk is full.
 */
int allocate_block() {
    for (int i = 1; i < DISK_BLOCKS; ++i) { // Start from 1, assume 0 is super block/reserved
        if (disk_free_list[i] == 0) {
            disk_free_list[i] = 1; // Mark as used
            return i;
        }
    }
    return -1; // Disk is full
}

/**
 * @brief Frees a disk block by setting its index in the free list to 0.
 */
void free_block(int block_index) {
    if (block_index >= 1 && block_index < DISK_BLOCKS) {
        disk_free_list[block_index] = 0;
    }
}

/**
 * @brief Allocates a free inode from the inode table.
 * @return The index of the allocated inode, or -1 if the inode table is full.
 */
int allocate_inode() {
    for (int i = 0; i < INODE_COUNT; ++i) {
        if (!inode_table[i].allocated) {
            inode_table[i].allocated = true;
            return i;
        }
    }
    return -1; // Inode table is full
}

/**
 * @brief Handles file creation: allocates inode and necessary data blocks.
 */
void create_file(const string& name, int size) {
    if (directory.count(name)) {
        cout << "\nERROR: File '" << name << "' already exists." << endl;
        return;
    }

    // 1. Calculate required blocks
    int required_blocks = (size + BLOCK_SIZE - 1) / BLOCK_SIZE;
    if (required_blocks > MAX_BLOCK_POINTERS) {
        cout << "\nERROR: File size requires " << required_blocks << " blocks, exceeding max direct pointers (" << MAX_BLOCK_POINTERS << ")." << endl;
        return;
    }

    // 2. Allocate Inode
    int inode_idx = allocate_inode();
    if (inode_idx == -1) {
        cout << "\nERROR: Inode table is full. Cannot create file." << endl;
        return;
    }
    
    // 3. Allocate Data Blocks and Update Inode
    vector<int> allocated_blocks;
    for (int i = 0; i < required_blocks; ++i) {
        int block_idx = allocate_block();
        if (block_idx == -1) {
            // Rollback: Free partially allocated blocks and the inode
            for(int b : allocated_blocks) free_block(b);
            inode_table[inode_idx].allocated = false;
            cout << "\nERROR: Disk is full. Cannot allocate " << required_blocks << " blocks." << endl;
            return;
        }
        allocated_blocks.push_back(block_idx);
        inode_table[inode_idx].direct_blocks[i] = block_idx;
    }

    // 4. Update Directory and Inode Metadata
    inode_table[inode_idx].file_name = name;
    inode_table[inode_idx].file_size = size;
    directory[name] = inode_idx;

    cout << "\nSUCCESS: File '" << name << "' created." << endl;
    cout << "  -> Inode Index: " << inode_idx << endl;
    cout << "  -> Blocks Used: ";
    for (int b : allocated_blocks) cout << b << " ";
    cout << endl;
}

/**
 * @brief Handles file deletion: frees blocks and the inode.
 */
void delete_file(const string& name) {
    if (!directory.count(name)) {
        cout << "\nERROR: File '" << name << "' not found." << endl;
        return;
    }

    int inode_idx = directory[name];
    Inode& inode = inode_table[inode_idx];

    // 1. Free Data Blocks
    int num_blocks = (inode.file_size + BLOCK_SIZE - 1) / BLOCK_SIZE;
    for (int i = 0; i < num_blocks; ++i) {
        int block_idx = inode.direct_blocks[i];
        if (block_idx != 0) { // Check for allocated block
            free_block(block_idx);
            inode.direct_blocks[i] = 0; // Clear pointer
        }
    }

    // 2. Free Inode and remove from Directory
    inode.allocated = false;
    inode.file_name = "";
    inode.file_size = 0;
    directory.erase(name);

    cout << "\nSUCCESS: File '" << name << "' deleted. Inode " << inode_idx << " and all blocks freed." << endl;
}

/**
 * @brief Simulates reading data by accessing the inode's block pointers.
 */
void read_file(const string& name) {
    if (!directory.count(name)) {
        cout << "\nERROR: File '" << name << "' not found." << endl;
        return;
    }

    int inode_idx = directory[name];
    const Inode& inode = inode_table[inode_idx];
    int num_blocks = (inode.file_size + BLOCK_SIZE - 1) / BLOCK_SIZE;
    
    cout << "\n--- Reading File: " << name << " (Size: " << inode.file_size << " Bytes) ---" << endl;
    cout << "Simulating read access via Inode " << inode_idx << "." << endl;

    for (int i = 0; i < num_blocks; ++i) {
        int block_idx = inode.direct_blocks[i];
        if (block_idx != 0) {
            // Simulation of reading the actual data block
            cout << "  -> Accessed Disk Block " << block_idx << " (Contains " 
                 << min(inode.file_size - i * BLOCK_SIZE, BLOCK_SIZE) << " Bytes of data)." << endl;
        }
    }
    cout << "---------------------------------------------------------" << endl;
}

/**
 * @brief Displays the current state of the file system tables.
 */
void display_state() {
    cout << "\n=========================================================" << endl;
    cout << "                   FILE SYSTEM STATE                     " << endl;
    cout << "=========================================================" << endl;

    // 1. Directory
    cout << "--- Directory (Filename -> Inode Index) ---" << endl;
    if (directory.empty()) {
        cout << "  <EMPTY>" << endl;
    } else {
        for (const auto& pair : directory) {
            cout << "  " << setw(15) << left << pair.first << " -> Inode " << pair.second << endl;
        }
    }

    // 2. Inode Table
    cout << "\n--- Inode Table (" << INODE_COUNT << " Total) ---" << endl;
    for (int i = 0; i < INODE_COUNT; ++i) {
        if (inode_table[i].allocated) {
            cout << "  Inode " << i << " [" << inode_table[i].file_name 
                 << ", Size: " << inode_table[i].file_size << "]: Blocks {";
            int num_blocks = (inode_table[i].file_size + BLOCK_SIZE - 1) / BLOCK_SIZE;
            for (int j = 0; j < num_blocks; ++j) {
                cout << inode_table[i].direct_blocks[j] << (j < num_blocks - 1 ? ", " : "");
            }
            cout << "}" << endl;
        } else {
            cout << "  Inode " << i << ": <FREE>" << endl;
        }
    }

    // 3. Disk Free List (Simulated FAT/Free Block Map)
    cout << "\n--- Disk Free List (" << DISK_BLOCKS << " Blocks) ---" << endl;
    int used_blocks = count(disk_free_list.begin(), disk_free_list.end(), 1);
    cout << "  Used: " << used_blocks << ", Free: " << DISK_BLOCKS - used_blocks << endl;
    cout << "  Block Status (0=Free, 1=Used):";
    for (int i = 1; i < DISK_BLOCKS; ++i) {
        if (i % 8 == 1) cout << "\n  ";
        cout << i << ":" << disk_free_list[i] << " ";
    }
    cout << endl;
    cout << "=========================================================" << endl;
}

// --- Main Program ---
int main() {
    cout << "Starting Simple File System Simulation." << endl;
    
    // --- DEMONSTRATION STEPS ---

    // 1. Initial State
    display_state();
    
    // 2. Create Files (Allocates Inodes and Blocks)
    create_file("report.txt", 150);       // Needs 2 blocks (150/128 = 1.17 -> 2)
    create_file("photo.jpg", 300);        // Needs 3 blocks (300/128 = 2.34 -> 3)
    create_file("backup.zip", 512);       // Needs 4 blocks (512/128 = 4 -> 4)
    create_file("temp.dat", 100);         // Needs 1 block

    // 3. Show State After Creation
    display_state();

    // 4. Read File (Simulating access via Inode)
    read_file("report.txt");
    read_file("photo.jpg");

    // 5. Delete File (Frees Blocks and Inode)
    delete_file("report.txt");

    // 6. Show State After Deletion (Check if blocks were freed)
    display_state();

    // 7. Attempt to create a new file, which should reuse the freed resources
    create_file("new_log.log", 200);      // Should reuse Inode 0 and freed blocks

    // 8. Final State
    display_state();

    return 0;
}
/*
--- Execution Instructions (MANDATORY) ---

1. SAVE the file as simple_file_system.cpp.
2. COMPILE the source code:
   g++ simple_file_system.cpp -o fs_sim
3. RUN the executable:
   ./fs_sim

--- Analysis Goal ---
The output demonstrates the relationship:
1. Directory maps filename to Inode Index.
2. Inode stores metadata (size) and pointers (direct_blocks).
3. Block Pointers link the file to the Disk Free List (FAT substitute).
4. Deletion correctly updates the Directory, Inode, AND the Disk Free List.
*/
