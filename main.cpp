#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <numeric> 
#include <list> 
#include <random>
#include <chrono>
#include <set>
#include <ctime>

const int N_PARTICLES = 30;
const int MAX_ITER = 700;

using namespace std;


struct Strip
{
    int height;
    int width;

    /* Position of bottom left corner */
    int x_position;
    int y_position;
};

struct Item
{ 
    int id;
    int height;
    int width;
    int area;

    /* Position of bottom left corner */
    int x_position;
    int y_position;

    int rotated; // zero if is not rotated

};

typedef vector<Item> Item_vector; 

struct Particle
{
    vector<int> permutation; // position

    int fitness; // fiftness of current permutation
    
    vector<int> pbest; // personal best
    
    int pbest_fitness; // fitness of personal best

    vector<float> velocity; // continous values for velocity
    
    
};

typedef vector<Particle> Swarm;

bool compareByArea(const Item &a, const Item &b)
{
    return a.area < b.area;
}

bool compareByFitness(const Particle &a, const Particle &b)
{
    return a.fitness < b.fitness;
}

void openInstance(string filepath, Item_vector &items, int &strip_width, int &max_height, int &n_items)
{ 

    ifstream file(filepath);
    Item item;
    
    if (!file.is_open())
    {
        exit(EXIT_FAILURE);
    }

    /* Saving first two lines */
    int col1, col2;


    file >> col1 >> col2;
    strip_width = col1;


    file >> col1;
    n_items = col1;
    
    
    
    
    /* Reading Line */
    while(file >> col1 >> col2)
    {
        item.width = col1;
        max_height+= col2; // Updating max height for setting upper bound.
        item.height = col2;
        item.area = col2 * col1;
        item.rotated = 0;
        items.push_back(item);
    }

    /* Sort by area */
    sort(items.begin(), items.end(), compareByArea);

    /* Set new ID's according to area */ 
    for (int i = 0; i <= items.size(); i++)
    {
        items[i].id = i+1;
    }    
    
    /* Finish */     
    file.close();    
    cout << "Instance ready!"<< endl;
}

void particle_init(int n_items, Swarm &swarm)
{
    Particle particle;

    random_device rd;  
    mt19937 g(rd());  



    /* Define Initial permutation (1,2,3,...n_items) */
    vector<int> permutation(n_items);
    iota(begin(permutation), end(permutation), 1);

    /* Shuffling permutation vector */
    shuffle(begin(permutation), end(permutation), g);

    particle.permutation = permutation;
    
    /* Define Initial velocity (0,0,0,...0) */
    vector<float> velocity(n_items,0);    
    particle.velocity = velocity;
    particle.fitness = 99999999;
    particle.pbest_fitness = 99999999;

    swarm.push_back(particle);

}

void showItemsInfo(Item_vector items)
{
    for (int i=0; i<items.size(); i++){
        cout << items[i].id << ' ' << items[i].height << ' ' << items[i].width << endl;     
    }
}

void showPermutation(vector<int> permutation)
{
    for (int i=0; i<permutation.size(); i++){
        cout << permutation[i];
        cout << ", ";
    }
    cout <<endl;
}
void showPermutationF(vector<float> permutation)
{
    for (int i=0; i<permutation.size(); i++){
        cout << permutation[i];
        cout << ", ";
    }
    cout <<endl;
}

void RecursePacking(Strip &s2, vector<int> &permutation, Item_vector &items)
{

    for (int i = 0; i < permutation.size(); i++)
    {
        int item_id = permutation[i]-1;

        if (s2.height >= items[item_id].height && s2.width >= items[item_id].width)
        {
            /* Place item */
            items[item_id].x_position = s2.x_position;
            items[item_id].y_position = s2.y_position;

            /* Generate new strip S3 */
            Strip s3 = {};
            s3.x_position = s2.x_position;
            s3.y_position = s2.y_position + items[item_id].height;
            s3.height = s2.height - items[item_id].height;
            s3.width = s2.width;

            /* Generate new strip S4 */
            Strip s4 = {};
            s4.x_position = s2.x_position + items[item_id].width;
            s4.y_position = s2.y_position;
            s4.height = items[item_id].height;
            s4.width = s2.width - items[item_id].width;

            /* Erase */
            permutation.erase(permutation.begin() + i);
            
            if (s3.width*s3.height>s4.height*s4.width)
            {
                s2=s3; RecursePacking(s2,permutation,items);
                s2=s4; RecursePacking(s2,permutation,items);
                
            }
            else
            {
                s2=s4; RecursePacking(s2,permutation,items);
                s2=s3; RecursePacking(s2,permutation,items);
        
            }   

        }
        else{
            continue;
        }
        
    }
    return;
        
        

}

void HR(Item_vector &items, Strip &s, int n_items, Particle &particle)
{

    vector<int> permutation = particle.permutation;
    int fitness = 0;
    int i = 0;

    while(!permutation.empty())
    {
        
        int item_id = permutation[i]-1;
        int original_height;
        
        if (items[item_id].height > items[item_id].width)
        {
            original_height = items[item_id].height;
            items[item_id].height = items[item_id].width;
            items[item_id].width = original_height;
            items[item_id].rotated = 1;
        }           

        /* it is already defined, but convenience */
        items[item_id].x_position = s.x_position;
        items[item_id].y_position = s.y_position; 

        fitness += items[item_id].height;         

        /* Resize strip S1 */
        Strip s1 = {};
        s1.y_position = s.y_position + items[item_id].height;
        s1.x_position = s.x_position;
        s1.height = s.height - items[item_id].height;
        s1.width = s.width;

        /* Generate new strip S2 */
        Strip s2 = {};
        s2.x_position = s.x_position + items[item_id].width;
        s2.y_position = s.y_position;
        s2.height = items[item_id].height;
        s2.width = s.width - items[item_id].width;

        s = s1;

        /* Erase */
        permutation.erase(permutation.begin() + i);

        RecursePacking(s2,permutation,items);

    }      
            
    particle.fitness = fitness;

}

void showRectangles(Item_vector items)
{
    /* Print all rectangles and its information */
    cout << "-> Printing rectangles of instance and its information!"<<endl;
    cout << "-------------------------------------------------------"<<endl;
    cout << "AreaID, X,      Y,     Width,  Height,  Rotated?"<<endl;
    
    for (int i = 0; i < items.size(); i++)
    {
        cout << items[i].id << "	" << items[i].x_position;
        cout << "	" << items[i].y_position << "	";
        cout << items[i].width << "	" << items[i].height << "	"; 
        cout << items[i].rotated <<endl;
    }
}

void showSwarm(Swarm swarm)
{
    /* print swarm */
    for (int i = 0; i < swarm.size(); i++)
    {
        //showPermutation(swarm[i].permutation);
        cout << "Fitness: " << swarm[i].fitness << endl;
        cout << "Velocity : ";
        for (int j=0; j< swarm[i].velocity.size(); j++){
                cout << swarm[i].velocity[j];
                cout << ", ";
            }
        cout <<endl;
        cout <<endl;
        
    } 
     
}


int main(int argc, const char *argv[]) 
{


    /* Taking algorithm time performance! */
    auto t1 = chrono::high_resolution_clock::now(); 

    cout << "Start!"<<endl;
    
    /* Save items data */
    Item_vector items;

    /* Open Instance File */

    /* Define number of items to pack */
    int n_items = 0; 
    int strip_width = 0;
    int max_height = 0;

    string filepath = argv[1];
    cout << "-> Opening instances!"<<endl;
    openInstance(filepath, items, strip_width, max_height, n_items);

    
    unsigned int a = filepath.rfind('.');
    unsigned int b = filepath.rfind('/',a-1);
    string name = filepath.substr(b+1,a-b-1);
    
    

    cout << "-> Number of items: " << n_items << endl;
    cout << "-> Strip width: " << strip_width << endl;
    cout << "-> Upper bound computed: "  << max_height << endl;

    /* Swarm of particles */
    Swarm swarm;

    /* Initialize particles */
    for(int i=0; i<N_PARTICLES; i++){
        //unsigned seed = i+1;
        particle_init(n_items, swarm);

        /* Initialize strip */
        Strip s = {};
        s.height = max_height;
        s.width = strip_width;

        s.x_position = 0;
        s.y_position = 0;
    
        /* Evaluate each particle */
        HR(items,s,n_items,swarm[i]); 

        swarm[i].pbest = swarm[i].permutation;
        swarm[i].pbest_fitness = swarm[i].fitness;

    } 

    //showSwarm(swarm);

    /* Declare global best particle */

    Particle gbest;
    gbest.fitness = max_height;


    /* Select best position */
    sort(swarm.begin(), swarm.end(), compareByFitness);
    
    if (swarm[0].fitness < gbest.fitness)
    {
        gbest.fitness = swarm[0].fitness;
        gbest.permutation = swarm[0].permutation;
    }

    int iter = 0;
    float omega = 1.2;
    float c1 = 2.05;
    float c2 = 2.05;
    float omega_max = 1.2;
    float omega_min = 0.7;

    float velocity_max = (n_items)/2;
    float velocity_min = -(n_items)/2;
    float current_velocity = 0;


    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0, 1);//uniform distribution between 0 and 1

    while (iter<MAX_ITER)
    {

        iter++;
        //cout << gbest.fitness << endl;
        
        /* Update velocity and position with results */
        for (int p = 0; p < N_PARTICLES; p++)
        {
            vector<float> new_permutation(n_items);
            iota(begin(new_permutation), end(new_permutation), 1);

            for (int i = 0; i < swarm[p].permutation.size(); i++)
            {
            
                float r1 = dis(gen);
                float r2 = dis(gen);

                float pfactor = swarm[p].pbest[i] - swarm[p].permutation[i];
                float gfactor = gbest.permutation[i] - swarm[p].permutation[i];                

                current_velocity = omega*swarm[p].permutation[i] + r1*c1*pfactor + r2*c2*gfactor;

                if (current_velocity < velocity_min)
                {
                    current_velocity = velocity_min;

                }else if(current_velocity > velocity_max)
                {
                    current_velocity = velocity_max;
                }

                swarm[p].velocity[i] = current_velocity;
            
                new_permutation[i] = swarm[p].permutation[i] + swarm[p].velocity[i];
                
            }

            /* Discretize position */
            vector<float> assignments = new_permutation;
            sort(assignments.begin(),assignments.end());

            for (int i = 0; i < new_permutation.size(); i++)
            {
                for (int j = 0; j < assignments.size(); j++)
                {
                    if (new_permutation[j]==assignments[i])
                    {
                        swarm[p].permutation[i] = j+1;
                    }   
                }
            }

            /* Initialize strip */
            Strip s = {};
            s.height = max_height;
            s.width = strip_width;

            s.x_position = 0;
            s.y_position = 0;
        
            /* Evaluate particle */
            HR(items,s,n_items,swarm[p]); 

            if (swarm[p].fitness < swarm[p].pbest_fitness)
            {
                swarm[p].pbest = swarm[p].permutation;
                swarm[p].pbest_fitness = swarm[p].fitness;
            }
            
            if (swarm[p].pbest_fitness < gbest.fitness)
            {
                gbest.fitness = swarm[p].pbest_fitness;
                gbest.permutation = swarm[p].pbest;
            }

        }
        
        omega = (omega_max)-((omega_max-omega_min)/MAX_ITER)*iter;
    } 
    

    //showSwarm(swarm);
    //showRectangles(items);

    //showPermutation(gbest);

    Strip s = {};
    s.height = max_height;
    s.width = strip_width;

    s.x_position = 0;
    s.y_position = 0;

    HR(items,s,n_items,gbest); 
    cout << "-> Fitness of best solution: " << gbest.fitness << endl;
    
    //showRectangles(items);


    //cout << "-> End!" << endl;

    auto t2 = chrono::high_resolution_clock::now(); 
    chrono::duration<double> time_span = (t2 - t1);
    cout << "-> It took me " << time_span.count() << " seconds.";
    cout << endl;

/*     ofstream myfile;
    myfile.open ("Results/"+name+".csv", fstream::app);
    myfile << gbest.fitness << "," << time_span.count() << endl;
    myfile.close(); */


    return 0;
}