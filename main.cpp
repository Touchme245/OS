#include <iostream>
#include <vector>
#include <math.h>
#include <pthread.h>
#include <chrono>
#include <cstdlib>
pthread_mutex_t mutex;

typedef struct{
    double x;
    double y;
    double z;
} Point;


Point point1;
Point point2;
Point point3;
double maxArea = -1;

typedef struct{
    int leftBorder;
    int rightBorder;
    int p1;
    int p2;
} ProcessData;


std::vector<Point> points = {
    {-2 , -1 , 0},
    {1 , 2 , 3},
    {4 , 5 , 6},
    {7 , 8 , 9},
    {10 , 11 , 12},
    {12 , 14 , 15},
    {16 , 17 , 18},
    {19 , 20 , 21},
    {22 , 23 , 24},
    {25 , 26 , 27},
    {28 , 29 , 30},
    {31 , 32 , 33},
    {34 , 35 , 36},
    {37 , 38 , 39},
    {40 , 41 , 42},
    {43 , 44 , 45},
    {46 , 47 , 48},
    {49 , 50 , 51},
    {52 , 53 , 54},
    {55 , 56 , 57}
};


double calculateArea(const Point p1, const Point p2,const Point p3){
    double a = sqrt(pow(p2.x - p1.x, 2) + pow(p2.y -p1.y, 2) + pow(p2.z - p1.z, 2));
    double b = sqrt(pow(p3.x - p2.x, 2) + pow(p3.y -p2.y, 2) + pow(p3.z - p2.z, 2));
    double c = sqrt(pow(p1.x - p3.x, 2) + pow(p1.y -p3.y, 2) + pow(p1.z - p3.z, 2));
    double polyPerimetr = (a + b + c) / 2;
    return sqrt(polyPerimetr * (polyPerimetr - a) * (polyPerimetr - b) * (polyPerimetr - c));
}

void* findMaxArea(void* data){
    Point tmp1;
    Point tmp2;
    Point tmp3;
    double localMaxArea  = -1;
    ProcessData processData = *((ProcessData*)data);
    int start = processData.leftBorder; 
    int finish = processData.rightBorder;
    Point p1  = points[processData.p1];
    Point p2 = points[processData.p2];
    double area;
    // double maxArea = -20;
   
    for (int i = start; i < finish; ++ i){
        area = calculateArea(p1, p2, points[i]);
         
        if (area > localMaxArea){
           
            localMaxArea = area;
            tmp1 = p1;
            tmp2 = p2;
            tmp3 = points[i];
            
        }
        
    }
    pthread_mutex_lock(&mutex);
    if (localMaxArea > maxArea){
        maxArea = localMaxArea;
        point1 = tmp1;
        point2 = tmp2;
        point3 = tmp3;
    }
    pthread_mutex_unlock(&mutex);

    pthread_exit(0);
}


int main(int argc, char*argv[]){
    int timeWithOneThread = 23564;
    int n;
    if (argc < 2){
        std::cout << "You need to provide amount of threads" << "\n";
        return 1;
    }
    for (int i = 0; i < 200; ++i){
    
        double x = rand() % 10000;
        double y = rand() % 10000;
        double z = rand() % 10000;
       
        points.push_back({x,y,z});

    }
    n = points.size();

    auto start = std::chrono::high_resolution_clock::now();
    const int threadCount = std::atoi(argv[1]);
    pthread_t tid[threadCount];

    unsigned long long  startTime = clock();

    for (int i = 0; i < points.size(); ++i){
        for (int j = i+1; j < points.size(); ++j){
            for (int k = 0; k < threadCount; ++k){
                ProcessData* data = new ProcessData();
                data->leftBorder = ceil(n / threadCount) * k;
                data->rightBorder = ceil(n / threadCount) * (k + 1);
                data->p1 = i;
                data->p2 = j;
            
                pthread_create(&tid[k], NULL, findMaxArea, data);
            }
            for (int z = 0; z < threadCount; ++z){
                pthread_join(tid[z], NULL);
            }
        }
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end-start);
    std::cout << "res Point 1: " << point1.x << " " << point1.y << " " << point1.z << "\n";
    std::cout << "res Point 2: " << point2.x << " " << point2.y << " " << point2.z << "\n";
    std::cout << "res Point 3: " << point3.x << " " << point3.y << " " << point3.z << "\n";

    std::cout << "Time" << duration.count() << "\n";
    std::cout << "Boost" << timeWithOneThread * 1.0 / duration.count() * 1.0 << "\n";
    

}
