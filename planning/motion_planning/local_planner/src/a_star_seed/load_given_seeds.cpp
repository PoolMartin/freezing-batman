//
//  a_star_seed.cpp
//  AStarSeed
//
//  Created by Satya Prakash on 06/09/13.
//  Copyright (c) 2013 Satya Prakash. All rights reserved.
//

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <a_star_seed/a_star_seed.hpp>

namespace navigation {

    void AStarSeed::loadGivenSeeds() {
        int vmax = 70;
        int max_iterations = 10000;
        int min_radius = 70;
        int num_seeds;
        int return_status;
        double x, y, z;

        node_handle.getParam("local_planner/vmax", vmax);
        node_handle.getParam("local_planner/max_iterations_load_given_seeds", max_iterations);
        node_handle.getParam("local_planner/min_rad", min_radius);

        //work TODO change to c++
        FILE *seeds_file = fopen(seeds_file_name.c_str(), "r");

        if (!seeds_file) {
            std::cout << "load in opening seed file : " << seeds_file_name << std::endl;
        }
        return_status = fscanf(seeds_file, "%d\n", &num_seeds);
        if (return_status == 0) {
            //ROS_ERROR("[PLANNER] Incorrect seed file format");
            //Planner::finBot();
            exit(1);
        }

        for (int i = 0; i < num_seeds; i++) {
            Seed seed;
            return_status = fscanf(seeds_file, "%lf %lf %lf %lf %lf\n", &seed.velocityRatio, &x, &y, &z, &seed.costOfseed);
            if (return_status == 0) {
                //                ROS_ERROR("[PLANNER] Incorrect seed file format");
                //                Planner::finBot();
                exit(1);
            }

            seed.leftVelocity = vmax * seed.velocityRatio / (1 + seed.velocityRatio);
            seed.rightVelocity = vmax / (1 + seed.velocityRatio);

            seed.final_state = State((int) x, (int) y, z, 0);

            int num_seed_points;
            return_status = fscanf(seeds_file, "%d\n", &num_seed_points);
            if (return_status == 0) {
                std::cout << "[PLANNER] Incorrect seed file format";
                exit(1);
            }

            for (int j = 0; j < num_seed_points; j++) {
                double temp_x, temp_y;
                return_status = fscanf(seeds_file, "%lf %lf \n", &temp_x, &temp_y);
                State point((int) temp_x, (int) temp_y, 0, 0);

                if (return_status == 0) {
                    //ROS_ERROR("[PLANNER] Incorrect seed file format");
                    exit(1);
                }

                seed.intermediatePoints.insert(seed.intermediatePoints.begin(), point);
            }
            givenSeeds.push_back(seed);
        }
    }

    void quickReflex::loadGivenSeeds(const State& start, const State& goal) {
        int v_max = 70;
        int number_of_seeds;
        int return_status;
        double x, y, z;
        int dt_constant =2;
        node_handle.getParam("local_planner/distance_transform_constant", dt_constant);
        node_handle.getParam("local_planner/vmax", v_max);
        FILE *textFileOFSeeds = fopen(seeds_file.c_str(), "r");

        if (!textFileOFSeeds) {
            std::cout << "load in opening seed file : " << seeds_file << std::endl;
        }

        return_status = fscanf(textFileOFSeeds, "%d\n", &number_of_seeds);

        if (return_status == 0) {
            //ROS_ERROR("[PLANNER] Incorrect seed file format");
            //Planner::finBot();
            exit(1);
        }

        for (int i = 0; i < number_of_seeds; i++) {
            Seed s;
            double cost = 0;
            double cost_DT = 0;
            return_status = fscanf(textFileOFSeeds, "%lf %lf %lf %lf\n", &s.velocityRatio, &x, &y, &z);
            if (return_status == 0) {
                // ROS_ERROR("[PLANNER] Incorrect seed file format");
                exit(1);
            }

            if (y > 0){
             s.leftVelocity = v_max * s.velocityRatio / (1 + s.velocityRatio);
             s.rightVelocity = v_max / (1 + s.velocityRatio);
            }
            else{
             s.leftVelocity = -v_max * s.velocityRatio / (1 + s.velocityRatio);
             s.rightVelocity = -v_max / (1 + s.velocityRatio);
            }
            s.final_state = State((int) x, (int) y, z, 0);

            int n_seed_points;
            return_status = fscanf(textFileOFSeeds, "%d\n", &n_seed_points);
            if (return_status == 0) {
                std::cout << "[PLANNER] Incorrect seed file format";
                exit(1);
            }

            for (int j = 0; j < n_seed_points; j++) {
                double tempXvalue, tempYvalue;
                return_status = fscanf(textFileOFSeeds, "%lf %lf\n", &tempXvalue, &tempYvalue);
                State point((int) tempXvalue, (int) tempYvalue, 0, 0);
                State point2((int) start.x() + tempXvalue, (int) start.y() + tempYvalue, 0, 0);

                if (return_status == 0) {
                    //ROS_ERROR("[PLANNER] Incorrect seed file format");
                    exit(1);
                }

                cost += point2.distanceTo(goal);
                cost_DT += fusion_map.at<uchar>(fusion_map.rows - (start.x() + tempXvalue) - 1, start.y() + tempYvalue);
                s.intermediatePoints.insert(s.intermediatePoints.begin(), point);
            }
            s.costOfseed = (cost / n_seed_points)/900 + ((cost_DT / n_seed_points)/255)/dt_constant + fabs(atan2f((goal.y()-start.y()), (goal.x()-start.x())) - atan2f(y, x))/M_PI;
            givenSeeds.push_back(s);
        }

        fclose(textFileOFSeeds);
    }
}