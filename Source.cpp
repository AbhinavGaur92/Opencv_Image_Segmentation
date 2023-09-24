#include <opencv2/opencv.hpp>
#include <iostream>


#include <vector>
#include <stack>
#include <map>
using namespace cv;
using namespace std;


// Initialize direction vectors
int dRow[] = { 1, 0 };
int dCol[] = { 0, 1 };

struct components_sort
{
    bool operator() (Vec3b a, Vec3b b) const
    {
        if (a[0] == b[0])
        {
            if (a[1] == b[1])
            {
                if (a[2] == b[2])
                    return true;
                else
                    return (a[2] < b[2]);
            }
            else
                return (a[1] < b[1]);
        }
        else
            return (a[0] < b[0]);
    }
};
map < Vec3b, vector<pair<int, int>> ,components_sort> components_map;
vector<pair< Vec3b, vector<pair<int, int>>> > components_vector;

bool isSameColor(Vec3b& color1, Vec3b& color2)
{
    ///*
    int diff = abs(color1[0] - color2[0]) +
        abs(color1[1] - color2[1]) +
        abs(color1[2] - color2[2]);

         return (diff <= 25);
        // */
    /*
    int diff = max(
        abs(color1[0] - color2[0]) ,
        max(abs(color1[1] - color2[1]) ,
            abs(color1[2] - color2[2])) );
   
    return (diff <= 25);
    */
    
}
// Function to check if mat[row][col]
// is unvisited and lies within the
// boundary of the given matrix
bool isValid(Mat3b& grid,vector<vector<bool>>& vis,
    int row, int col, Vec3b& color)
{
    // If cell is out of bounds
    if (row < 0 || col < 0
        || row >= grid.rows || col >= grid.cols )
        return false;

    // If the cell is already visited
    if (vis[row][col])
        return false;

    Vec3b& color2 = grid.at<Vec3b>(row, col);

    if (!isSameColor(color, color2))
        return false;
    // Otherwise, it can be visited
    return true;
}

// Function to perform DFS
// Traversal on the matrix grid[]
void DFS(int row, int col,
    Mat3b& grid,
    vector<vector<bool>>& vis, Vec3b& color, int& count, vector<pair<int, int>>& component)
{

    // Mark this cell as visited
    vis[row][col] = true;
    count += 1;
    component.push_back({ row,col });

    //grid.at<Vec3b>(Point(col, row)) = color;

    // Recur for all connected neighbours
    for (int k = 0; k < 2; ++k)
        if (isValid(grid, vis,row + dRow[k], col + dCol[k],color))
            DFS( row + dRow[k], col + dCol[k], grid, vis,color, count, component);

}
void drawComp(Mat3b& image, Vec3b color, vector<pair<int, int>>& component)
{
    for (auto temp : component)
    {
        image.at<Vec3b>(Point(temp.second, temp.first)) = color;
    }
}
void run_seg()
{
    //components_map.clear();
    components_vector.clear();
    Mat3b image = imread("C:\\Users\\abhin\\Documents\\work\\2_test.jpg");
    vector<vector<bool>> visited(image.rows, vector<bool>(image.cols,false));
    //cout <<"run_seg() " << endl;
    for (int y = 0; y < image.rows; y++)
    {
        for (int x = 0; x < image.cols; x++)
        {
            if (visited[y][x])
                continue;
            Vec3b color = image.at<Vec3b>(y, x);
            int count = 0;
            vector<pair<int, int>> component;
            DFS(y, x, image, visited, color,count, component);
            components_vector.push_back({ image.at<Vec3b>(y, x),component });
            /*
            // get pixel
            Vec3b& color = image.at<Vec3b>(y, x);

            // ... do something to the color ....
            color[0] = 13;
            color[1] = 13;
            color[2] = 13;

            // set pixel
            //image.at<Vec3b>(Point(x,y)) = color;
            //if you copy value
            */
        }
    }
    //imshow("image", image);


    int num_components = components_vector.size();
    vector<bool> merged(num_components, false);
    Mat3b seg_img = Mat::zeros(Size(image.cols, image.rows), CV_8UC3);
    for (int i = 0; i < num_components - 1; i++)
    {
        if (merged[i])
            continue;
        int component_size = components_vector[i].second.size();
        seg_img = Mat::zeros(Size(image.cols, image.rows), CV_8UC3);
        drawComp(seg_img, components_vector[i].first, components_vector[i].second);
        for (int j = i+1; j < num_components; j++)
        {
            if (merged[j])
                continue;
            if (isSameColor(components_vector[i].first, components_vector[j].first))
            {
                drawComp(seg_img, components_vector[j].first, components_vector[j].second);
                merged[j] = true;
                component_size+= components_vector[j].second.size();
            }
            
        }
        if (component_size >=  250)
        {
            //imshow("seg_img", seg_img);
            imwrite("seg_img_" + to_string(i) + ".png", seg_img);
            //waitKey();
        }
        
        merged[i] = true;
    }
}
int main()
{
    run_seg();
    
    return 0;
}
