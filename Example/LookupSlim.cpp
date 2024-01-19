/**
 * @file LookupSlim.cpp
 * @author fugu133 (@fugumarine)
 * @brief SLIMの電波を見たいときに使う何か
 * @version 0.1
 * @date 2024-01-19
 * 
 * @copyright Copyright (c) 2024

 */

#include <SatFind/Core>
#include <fstream>

using namespace satfind;

int main() {
    auto start_dt = DateTime{"2024-01-20T00:00:00+09:00"};
    auto end_dt = DateTime{"2024-01-20T01:00:00+09:00"};
    const auto delta_t = Seconds(0);
    auto antenna_pos = Wgs84Position{Degree{136.7}, Degree{36.5}, 100}; // 適当なアンテナ位置
    auto gs = GroundObserver(antenna_pos);

    for (auto dt = start_dt; dt < end_dt; dt += Minutes(1)) {
        auto moon_pos = MoonPosition(dt, delta_t);
        auto antenna_dir = gs.lookUpPosition(moon_pos.eci());
        std::cout << antenna_dir << std::endl;
    }
}