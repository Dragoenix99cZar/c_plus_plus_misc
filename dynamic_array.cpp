#include <iostream>
#include <initializer_list>
#include <stdexcept>
#include <cstring>

// Internal representation of a Dynamic Array
struct Vector {
    int* data = nullptr;        // Pointer to memory buffer
    size_t count = 0;           // Number of elements currently stored
    size_t capacity = 0;        // Allocated capacity
    size_t stride = sizeof(int); // Size of each element in bytes
};

// Initialize or resize capacity
void reserve(Vector& vec, size_t new_capacity) {
    if (new_capacity <= vec.capacity) return;

    int* new_data = new int[new_capacity];
    if (vec.data) {
        std::memcpy(new_data, vec.data, vec.count * vec.stride);
        delete[] vec.data;
    }
    vec.data = new_data;
    vec.capacity = new_capacity;
}

// Add element (1D push_back)
void add(Vector& vec, int val) {
    if (vec.count >= vec.capacity) {
        size_t next_cap = (vec.capacity == 0) ? 4 : vec.capacity * (float)1.5;
        reserve(vec, next_cap);
    }
    vec.data[vec.count++] = val;
}

// Extend vector by appending elements of another vector
void extend(Vector& vec, const Vector& other) {
    if (other.count == 0) return;
    reserve(vec, vec.count + other.count);
    std::memcpy(vec.data + vec.count, other.data, other.count * vec.stride);
    vec.count += other.count;
}

// Remove element at flat index
void remove(Vector& vec, size_t index) {
    if (index >= vec.count) throw std::out_of_range("Index out of bounds");
    for (size_t i = index; i < vec.count - 1; ++i) {
        vec.data[i] = vec.data[i + 1];
    }
    --vec.count;
}

// Read element at flat index
int read(const Vector& vec, size_t index) {
    if (index >= vec.count) throw std::out_of_range("Index out of bounds");
    return vec.data[index];
}

// Helper to calculate flat index from N-dimensional coordinates
// Formula: index = (((d0 * dim1 + d1) * dim2 + d2) * dim3 + d3)
size_t get_flat_index(const std::initializer_list<size_t>& coords, const std::initializer_list<size_t>& dims) {
    auto c_it = coords.begin();
    auto d_it = dims.begin();

    size_t index = 0;
    for (; c_it != coords.end() && d_it != dims.end(); ++c_it, ++d_it) {
        if (*c_it >= *d_it) throw std::out_of_range("Coordinate out of dimension bounds");
        index = index * (*d_it) + (*c_it);
    }
    return index;
}

// Multi-dimensional Read wrapper
int read_nd(const Vector& vec, std::initializer_list<size_t> coords, std::initializer_list<size_t> dims) {
    size_t flat_idx = get_flat_index(coords, dims);
    return read(vec, flat_idx);
}

// Display N-Dimensional Data Recursively
void display_recursive(const Vector& vec, const size_t* dims, size_t num_dims, size_t current_dim, size_t& offset) {
    if (current_dim == num_dims - 1) {
        std::cout << "[";
        for (size_t i = 0; i < dims[current_dim]; ++i) {
            std::cout << vec.data[offset++];
            if (i < dims[current_dim] - 1) std::cout << ", ";
        }
        std::cout << "]";
        return;
    }

    std::cout << "[\n";
    for (size_t i = 0; i < dims[current_dim]; ++i) {
        // Indentation for visualization
        for (size_t tab = 0; tab <= current_dim; ++tab) std::cout << "  ";
        display_recursive(vec, dims, num_dims, current_dim + 1, offset);
        if (i < dims[current_dim] - 1) std::cout << ",\n";
    }
    std::cout << "\n";
    for (size_t tab = 0; tab < current_dim; ++tab) std::cout << "  ";
    std::cout << "]";
}

void display(const Vector& vec, std::initializer_list<size_t> shape) {
    if (vec.count == 0) {
        std::cout << "[]\n";
        return;
    }
    size_t total_elements = 1;
    for (size_t d : shape) total_elements *= d;

    if (total_elements != vec.count) {
        std::cout << "Shape does not match total vector size (" << vec.count << " elements)\n";
        return;
    }

    size_t offset = 0;
    display_recursive(vec, shape.begin(), shape.size(), 0, offset);
    std::cout << "\n";
}

// Free allocated memory
void free_vector(Vector& vec) {
    delete[] vec.data;
    vec.data = nullptr;
    vec.count = 0;
    vec.capacity = 0;
}

void stats(const Vector& vec){
    std::cout << "\n--------------------------";
    std::cout << "\ncount    : " << vec.count;
    std::cout << "\ncapacity : " << vec.capacity;
    std::cout << "\nstride   : " << vec.stride;
    std::cout << "\n--------------------------\n";
}

int main() {
    Vector vec = {0};

    std::cout << "--- Vector Initialize to {0} ---\n";
    display(vec, {vec.count});
    stats(vec);
    std::cout << "--------------------------------\n";

    // 1. Add elements
    for (int i = 1; i <= 48; i += 1) {
        add(vec, i * 3);
    }
    stats(vec);
    // 2. Read element at 2D coordinate (row 1, col 2) for shape {4, 6}
    std::cout << "2D Read (1, 2) in {4, 6}: " << read_nd(vec, {1, 2}, {4, 6}) << "\n\n";

    // 3. Display as 1D, 2D, 3D, and 4D structures
    std::cout << "--- 1D Representation {48} ---\n";
    display(vec, {48});

    std::cout << "\n--- 2D Representation {4, 6} ---\n";
    display(vec, {4, 12});

    std::cout << "\n--- 3D Representation {2, 3, 4} ---\n";
    display(vec, {2, 3, 8});

    std::cout << "\n--- 4D Representation {2, 2, 2, 3} ---\n";
    display(vec, {2, 2, 2, 2, 3});

    // 4. Extend Vector
    Vector extra;
    std::cout << "Vector extra;";
    stats(extra);
    add(extra, 99);
    std::cout << "add(extra, 99);";
    stats(extra);
    add(extra, 100);
    std::cout << "add(extra, 100);";
    stats(extra);
    add(extra, 151);
    std::cout << "add(extra, 151);";
    stats(extra);
    add(extra, 152);
    std::cout << "add(extra, 152);";
    stats(extra);
    add(extra, 153);
    std::cout << "add(extra, 153);";
    stats(extra);


    std::cout << "vec";
    stats(vec);
    std::cout << "extra";
    stats(extra);
    extend(vec, extra);
    std::cout << "extend(vec, extra);";
    std::cout << "\nextra";
    stats(extra);
    std::cout << "vec";
    stats(vec);

    std::cout << "\n--- After Extend (Added 99, 100) ---\n";
    std::cout << "New 1D count: " << vec.count << "\n";
    display(vec, {vec.count});
    stats(vec);

    // 5. Remove element
    int removeIdx = 10;
    std::cout << "\n--- Before Remove (Index " << removeIdx << " removed) ---\n";
    std::cout << "Element at index " << removeIdx << " is now: " << read(vec, removeIdx) << "\n";
    remove(vec, removeIdx); // Remove first element
    stats(vec);
    std::cout << "\n--- After Remove (Index " << removeIdx << " removed) ---\n";
    std::cout << "Element at index " << removeIdx << " is now: " << read(vec, removeIdx) << "\n";
    display(vec, {vec.count});

    // Cleanup
    free_vector(vec);
    free_vector(extra);
    return 0;
}
