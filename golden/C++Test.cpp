#include <bits/stdc++.h>
using namespace std;

unsigned char* image_s = NULL; // source image array
unsigned char* image_t = NULL; // target image array
int width_s = 0;               // image width
int height_s = 0;              // image height
int width_t = 0;               // image width
int height_t = 0;              // image height
unsigned int rgb_raw_data_offset_s = 0; // RGB raw data offset
unsigned int bit_per_pixel_s = 0;      // bit per pixel
unsigned int byte_per_pixel_s = 0;    // byte per pixel
unsigned int rgb_raw_data_offset_t = 0; // RGB raw data offset
unsigned int bit_per_pixel_t = 0;      // bit per pixel
unsigned int byte_per_pixel_t = 0;    // byte per pixel

const int filter[3][3] = {
    {1,2,1},
    {2,4,2},
    {1,2,1}
};
const int factor = 16;

int read_input_bmp() {
    FILE* fp_s = fopen("input.bmp", "rb");
    if (fp_s == NULL) {
        std::cerr << "fopen fp_s error" << std::endl;
        return -1;
    }

    fseek(fp_s, 10, SEEK_SET);
    assert(fread(&rgb_raw_data_offset_s, sizeof(unsigned int), 1, fp_s));

    fseek(fp_s, 18, SEEK_SET);
    assert(fread(&width_s, sizeof(unsigned int), 1, fp_s));
    assert(fread(&height_s, sizeof(unsigned int), 1, fp_s));

    fseek(fp_s, 28, SEEK_SET);
    assert(fread(&bit_per_pixel_s, sizeof(unsigned short), 1, fp_s));
    byte_per_pixel_s = bit_per_pixel_s / 8;

    fseek(fp_s, rgb_raw_data_offset_s, SEEK_SET);

    size_t size = width_s * height_s * byte_per_pixel_s;
    image_s = reinterpret_cast<unsigned char*>(new void* [size]);
    if (image_s == NULL) {
        std::cerr << "allocate image_s error" << std::endl;
        return -1;
    }

    image_t = reinterpret_cast<unsigned char*>(new void* [size]);
    if (image_t == NULL) {
        std::cerr << "allocate image_t error" << std::endl;
        return -1;
    }

    assert(fread(image_s, sizeof(unsigned char),
        (size_t)(long)width_s * height_s * byte_per_pixel_s, fp_s));
    fclose(fp_s);

    return 0;
}


int read_output_bmp() {
    FILE* fp_s = fopen("output.bmp", "rb");
    if (fp_s == NULL) {
        std::cerr << "fopen fp_s error" << std::endl;
        return -1;
    }

    fseek(fp_s, 10, SEEK_SET);
    assert(fread(&rgb_raw_data_offset_t, sizeof(unsigned int), 1, fp_s));

    fseek(fp_s, 18, SEEK_SET);
    assert(fread(&width_t, sizeof(unsigned int), 1, fp_s));
    assert(fread(&height_t, sizeof(unsigned int), 1, fp_s));

    fseek(fp_s, 28, SEEK_SET);
    assert(fread(&bit_per_pixel_t, sizeof(unsigned short), 1, fp_s));
    byte_per_pixel_t = bit_per_pixel_t / 8;

    fseek(fp_s, rgb_raw_data_offset_t, SEEK_SET);

    size_t size = width_t * height_t * byte_per_pixel_t;
    image_t = reinterpret_cast<unsigned char*>(new void* [size]);
    if (image_t == NULL) {
        std::cerr << "allocate image_s error" << std::endl;
        return -1;
    }

    image_t = reinterpret_cast<unsigned char*>(new void* [size]);
    if (image_t == NULL) {
        std::cerr << "allocate image_t error" << std::endl;
        return -1;
    }

    assert(fread(image_t, sizeof(unsigned char),
        (size_t)(long)width_t * height_t * byte_per_pixel_t, fp_s));
    fclose(fp_s);

    return 0;
}
string dec_to_hex(int num) {
    string ans = "0x";
    string hex[16] = { "0","1","2","3","4","5","6","7","8","9","a","b","c","d","e","f" };
    ans += hex[num / 16];
    ans += hex[num % 16];

    return ans;
}
int main() {
    int cnt = 0;
    bool pass = true;
    read_input_bmp();
    cout << "width_s :" << width_s << endl;
    cout << "height_s :" << height_s << endl;
    cout << "width_s x height_s: " << width_s * height_s << endl;


    read_output_bmp();
    cout << "width_t :" << width_t << endl;
    cout << "height_t :" << height_t << endl;
    cout << "width_t x height_t: " << width_t * height_t << endl;

    for (int i = 0; i < width_t; i++) {
        for (int j = 0; j < height_t; j++) {
            int R = 0, G = 0, B = 0;
            for (int y = -1; y <= 1; y++) {
                for (int x = -1; x <= 1; x++) {
                    if (0 <= i + y && i + y < height_t && 0 <= j + x && j + x < width_t) {
                        R += filter[x + 1][y + 1] * (int)image_s[byte_per_pixel_s * ((i + y) * width_t + j + x) + 2];
                        G += filter[x + 1][y + 1] * (int)image_s[byte_per_pixel_s * ((i + y) * width_t + j + x) + 1];
                        B += filter[x + 1][y + 1] * (int)image_s[byte_per_pixel_s * ((i + y) * width_t + j + x) + 0];
                    }
                }
            }
            R /= factor;
            G /= factor;
            B /= factor;
            if (R != (int)image_t[byte_per_pixel_s * (i* width_t + j) + 2]) {
                cout << "pixel R:" << i << " " << j << " Error!!!" << endl;
                cout << "    Current result     : " << R << endl;
                cout << "    Original should be : " << (int)image_s[i * width_t + j + 2] << endl;
                cout << "    Correct should be  : " << (int)image_t[i * width_t + j + 2] << endl;
                pass = false;
            }

            if (G != (int)image_t[byte_per_pixel_s * (i * width_t + j) + 1]) {
                cout << "pixel G:" << i << " " << j << " Error!!!" << endl;
                cout << "    Current result     : " << G << endl;
                cout << "    Original should be : " << (int)image_s[i * width_t + j + 1] << endl;
                cout << "    Correct should be  : " << (int)image_t[i * width_t + j + 1] << endl;
                pass = false;
            }

            if (B != (int)image_t[byte_per_pixel_s * (i * width_t + j) + 0]) {
                cout << "pixel B:" << i << " " << j << " Error!!!" << endl;
                cout << "    Current result     : " << B << endl;
                cout << "    Original should be : " << (int)image_s[i * width_t + j + 0] << endl;
                cout << "    Correct should be  : " << (int)image_t[i * width_t + j + 0] << endl;
                pass = false;
            }
        }
    }
    if (pass == true) {
        cout << "All PASS" << endl;
    }
    else {
        cout << "Failed" << endl;
    }
    fstream file;
    file.open("input_bitmap.h", ios::out);
    file << "int width = " << width_s << ";" << endl;
    file << "int height = " << height_s << ";" << endl;
    file << "int byte_per_pixel = " << byte_per_pixel_s << ";" << endl;
    file << "unsigned char input_bmp[] = {" << endl;
    for (int i = 0; i < width_s * height_s * byte_per_pixel_s; i++) {
        file << dec_to_hex((int)image_s[i]);
        if (i != width_s * height_s * byte_per_pixel_s - 1)
            file << ", ";
        if (i % 12 == 11)
            file << endl;
    }
    file << "};";
    file.close();

    file.open("output_bitmap.h", ios::out);
    file << "int width = " << width_t << ";" << endl;
    file << "int height = " << height_t << ";" << endl;
    file << "int byte_per_pixel = " << byte_per_pixel_t << ";" << endl;
    file << "unsigned char output_bmp[] = {" << endl;
    for (int i = 0; i < width_t * height_t * byte_per_pixel_t; i++) {
        file << dec_to_hex((int)image_t[i]);
        if (i != width_t * height_t * byte_per_pixel_t - 1)
            file << ", ";
        if (i % 12 == 11)
            file << endl;
    }
    file << "};";
    file.close();
}

