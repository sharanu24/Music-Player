#include <iostream>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <ctime>

using namespace std;

// Node structure for doubly linked list
struct node
{
    char song[100];
    char singer[100]; // To store the singer's name
    time_t timestamp; // To store the timestamp
    int searchCount;  // To count the number of searches
    struct node *next;
    struct node *prev;
} *top, *temp, *top1, *rear;

// Function to write song, singer, and timestamp to file
void tofile(const char a[], const char s[], time_t timestamp)
{
    fstream f1;
    f1.open("playlist.txt", ios::out | ios::app);
    f1 << a << " | Singer: " << s << " | Added at: " << ctime(&timestamp); // Write song, singer, and timestamp to file
    f1.close();
}

bool isDuplicate(struct node *first, const char *song) {
    struct node *temp = first;
    while (temp != NULL) {
        if (strcmp(temp->song, song) == 0) {
            return true; // Song is already in the list
        }
        temp = temp->next;
    }
    return false;
}

// Function to add a song to the playlist (enqueue)
void enqueue(struct node *&first) {
    char a[100], s[100];
    cout << "\nEnter the song you want to insert: ";
    cin >> a;
    cout << "Enter the singer's name: ";
    cin >> s;

    if (isDuplicate(first, a)) {
        cout << "\nSong already exists in the list!";
        return;
    }

    struct node *new_node = (struct node*)malloc(sizeof(struct node));
    strcpy(new_node->song, a);
    strcpy(new_node->singer, s);
    new_node->next = NULL;
    new_node->prev = NULL;
    new_node->timestamp = time(0); // Set current time
    new_node->searchCount = 0;     // Initialize search count

    if (first == NULL) {
        first = new_node;
        rear = new_node;
    } else {
        rear->next = new_node;
        new_node->prev = rear;
        rear = new_node;
    }

    tofile(a, s, new_node->timestamp);
}

// Function to remove a song (dequeue)
void dequeue(struct node *&first)
{
    if (first == NULL) {
        cout << "\nThe playlist is empty.";
        return;
    }

    struct node *temp = first;
    cout << "\nDequeued Song: " << first->song;

    first = first->next;
    if (first != NULL)
        first->prev = NULL;
    else
        rear = NULL;

    free(temp);
}

// Function to add a song to the top (push)
void push(struct node *&first)
{
    char a[100], s[100];
    cout << "\nEnter the song you want to insert: ";
    cin >> a;
    cout << "Enter the singer's name: ";
    cin >> s;

    struct node *new_node = (struct node*)malloc(sizeof(struct node));
    strcpy(new_node->song, a);
    strcpy(new_node->singer, s);
    new_node->next = NULL;
    new_node->prev = NULL;
    new_node->timestamp = time(0); // Set current time
    new_node->searchCount = 0;     // Initialize search count

    if (first == NULL) {
        first = new_node;
    } else {
        new_node->next = first;
        first->prev = new_node;
        first = new_node;
    }
    tofile(a, s, new_node->timestamp);
}

// Function to remove a song from the top (pop)
void pop(struct node *&first)
{
    if (first == NULL) {
        cout << "\nThe playlist is empty.";
        return;
    }

    struct node *temp = first;
    cout << "\nPopped Song: " << first->song;

    first = first->next;
    if (first != NULL)
        first->prev = NULL;

    free(temp);
}

// Function to display the playlist
void display(struct node *first)
{
    while (first != NULL)
    {
        cout << "\n" << first->song << " | Singer: " << first->singer << " | Searches: " << first->searchCount << " | Added at: " << ctime(&first->timestamp);
        first = first->next;
    }
}

// Function to remove a specific song from the list
void remove_node(struct node *&first, char name[])
{
    struct node *temp1;
    temp1 = first;

    while (temp1 != NULL)
    {
        if (strcmp(temp1->song, name) == 0)
        {
            if (temp1 == first) // Removing the first node
            {
                first = first->next;
                if (first != NULL)
                    first->prev = NULL;
            }
            else if (temp1->next == NULL) // Removing the last node
            {
                temp1->prev->next = NULL;
                rear = temp1->prev;
            }
            else // Removing a node in between
            {
                temp1->prev->next = temp1->next;
                temp1->next->prev = temp1->prev;
            }
            free(temp1);
            cout << "\nSong removed successfully!";
            return;
        }
        temp1 = temp1->next;
    }
    cout << "\nSong not found.";
}

// Function to search for a song in the playlist
void search(char name[])
{
    temp = top;
    while (temp != NULL)
    {
        if (strcmp(temp->song, name) == 0)
        {
            temp->searchCount++; // Increment the search count
            cout << "\nSong Found: " << temp->song << " | Singer: " << temp->singer << " | Searches: " << temp->searchCount << " | Added at: " << ctime(&temp->timestamp);
            return;
        }
        temp = temp->next;
    }
    cout << "\nSong not found";
}

// Function to reverse the linked list and return the new head
struct node* reverseList(struct node *first)
{
    struct node *current = first;
    struct node *temp = NULL;

    // Swap the next and prev pointers for each node
    while (current != NULL)
    {
        temp = current->prev;
        current->prev = current->next;
        current->next = temp;
        current = current->prev;
    }

    // Adjust the head pointer to point to the new first node
    if (temp != NULL)
    {
        first = temp->prev;
    }

    cout << "\nPlaylist has been reversed!";
    return first; // Return the new head of the reversed list
}

// Function to sort the playlist alphabetically by song name
void sortByName(struct node *first)
{
    struct node *temp2;
    char a[100];
    while (first->next != NULL)
    {
        temp2 = first->next;
        while (temp2 != NULL)
        {
            if (strcmp(first->song, temp2->song) > 0)
            {
                strcpy(a, first->song);
                strcpy(first->song, temp2->song);
                strcpy(temp2->song, a);
            }
            temp2 = temp2->next;
        }
        first = first->next;
    }
}

// Function to sort the playlist by timestamp (time)
void sortByTime(struct node *first)
{
    struct node *temp2;
    struct node *current = first;
    while (current != NULL)
    {
        temp2 = current->next;
        while (temp2 != NULL)
        {
            if (difftime(current->timestamp, temp2->timestamp) > 0)
            {
                // Swap the songs and timestamps
                char tempSong[100];
                char tempSinger[100];
                time_t tempTime;

                strcpy(tempSong, current->song);
                strcpy(current->song, temp2->song);
                strcpy(temp2->song, tempSong);

                strcpy(tempSinger, current->singer);
                strcpy(current->singer, temp2->singer);
                strcpy(temp2->singer, tempSinger);

                tempTime = current->timestamp;
                current->timestamp = temp2->timestamp;
                temp2->timestamp = tempTime;
            }
            temp2 = temp2->next;
        }
        current = current->next;
    }
}

// Function to list all songs by a specific singer
void listSongsBySinger(struct node *first, const char *singerName) {
    bool found = false;
    while (first != NULL) {
        if (strcmp(first->singer, singerName) == 0) {
            cout << "\n" << first->song << " | Added at: " << ctime(&first->timestamp);
            found = true;
        }
        first = first->next;
    }
    if (!found) {
        cout << "\nNo songs found for singer: " << singerName;
    }
}

// Function to find and display the most searched song
void mostSearchedSong(struct node *first)
{
    if (first == NULL) {
        cout << "\nThe playlist is empty.";
        return;
    }

    struct node *mostSearched = first;
    while (first != NULL) {
        if (first->searchCount > mostSearched->searchCount) {
            mostSearched = first;
        }
        first = first->next;
    }

    if (mostSearched->searchCount > 0) {
        cout << "\nMost Searched Song: " << mostSearched->song << " | Searches: " << mostSearched->searchCount;
    } else {
        cout << "\nNo song has been searched yet.";
    }
}

int main()
{
    int ch;
    char song[100], singer[100];

    while (1)
    {
        cout << "\n\nPlaylist Management Menu";
        cout << "\n1. Add Song";
        cout << "\n2. Display Playlist";
        cout << "\n3. Remove Song from Front";
        cout << "\n4. Add Song to Top";
        cout << "\n5. Remove Song from Top";
        cout << "\n6. Remove Specific Song";
        cout << "\n7. Search Song";
        cout << "\n8. Sort Playlist by Name";
        cout << "\n9. Sort Playlist by Time";
        cout << "\n10. Reverse Playlist";
        cout << "\n11. List Songs by Singer";
        cout << "\n12. Most Searched Song";
        cout << "\n13. Exit";
        cout << "\nEnter your choice: ";
        cin >> ch;

        switch (ch)
        {
            case 1:
                enqueue(top);
                break;

            case 2:
                display(top);
                break;

            case 3:
                dequeue(top);
                break;

            case 4:
                push(top);
                break;

            case 5:
                pop(top);
                break;

            case 6:
                cout << "\nEnter the song to remove: ";
                cin >> song;
                remove_node(top, song);
                break;

            case 7:
                cout << "\nEnter the song to search: ";
                cin >> song;
                search(song);
                break;

            case 8:
                sortByName(top);
                break;

            case 9:
                sortByTime(top);
                break;

            case 10:
               top = reverseList(top); // Assign the returned head to top
                break;

            case 11:
                cout << "\nEnter the singer's name: ";
                cin >> singer;
                listSongsBySinger(top, singer);
                break;

            case 12:
                mostSearchedSong(top);
                break;

            case 13:
                exit(0);

            default:
                cout << "\nInvalid choice!";
        }
    }
    return 0;
}
