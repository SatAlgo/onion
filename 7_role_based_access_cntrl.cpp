#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>

using namespace std;

// Define the available system actions (Permissions)
using Permission = string;
// Define the user groups (Roles)
using Role = string;
// Define the user accounts (Users)
using User = string;
// Define the files/resources in the system
using Resource = string;

// --- RBAC Mapping Tables ---

// 1. Role-to-Permission Mapping: What actions can a role perform?
map<Role, set<Permission>> ROLE_PERMISSIONS = {
    {"Admin",       {"read", "write", "delete", "configure"}},
    {"Developer",   {"read", "write"}},
    {"Guest",       {"read"}}
};

// 2. User-to-Role Mapping: Which roles does a user possess?
map<User, set<Role>> USER_ROLES = {
    {"Alice",       {"Admin"}},
    {"Bob",         {"Developer"}},
    {"Charlie",     {"Developer", "Guest"}}, 
    {"Eve",         {"Guest"}}
};

// --- Core RBAC Authorization Function ---

// Checks if a given user has the required permission for a resource
bool check_access(const User& user, const Permission& required_permission, const Resource& resource) {
    cout << "\n[AUTH CHECK] User '" << user << "' wants to '" << required_permission << "' " << resource << "..." << endl;

    // 1. Find the roles associated with the user
    if (USER_ROLES.find(user) == USER_ROLES.end()) {
        cout << "Access Denied: User not found." << endl;
        return false;
    }

    const set<Role>& user_roles = USER_ROLES.at(user);
    
    // 2. Check if ANY of the user's roles grants the required permission
    for (const Role& role : user_roles) {
        // Check if the role exists in the permissions map
        if (ROLE_PERMISSIONS.count(role)) {
            const set<Permission>& role_perms = ROLE_PERMISSIONS.at(role);
            
            // Check if the required permission is in the role's set
            if (role_perms.count(required_permission)) {
                cout << "Access Granted: Authorized via role '" << role << "'." << endl;
                return true; // Access granted by this role
            }
        }
    }

    // 3. If no role grants the permission
    cout << "Access Denied: No associated role has the required permission." << endl;
    return false;
}

// --- Main Demonstration ---
int main() {
    // --- Define Resources ---
    Resource system_config = "System Config File";
    Resource project_code = "Project Source Code";
    Resource public_report = "Public Report";

    // --- Test Scenarios ---

    // Scenario 1: Alice (Admin) attempts high-level operation
    check_access("Alice", "configure", system_config); 

    // Scenario 2: Bob (Developer) attempts standard operation
    check_access("Bob", "write", project_code); 

    // Scenario 3: Charlie (Developer) attempts an Admin-only operation
    check_access("Charlie", "delete", system_config); 
    
    // Scenario 4: Eve (Guest) attempts a read (allowed)
    check_access("Eve", "read", public_report); 
    
    // Scenario 5: Eve (Guest) attempts to write (denied)
    check_access("Eve", "write", project_code); 
    
    return 0;
}
