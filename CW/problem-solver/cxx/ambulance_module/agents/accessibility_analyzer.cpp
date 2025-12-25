#include "accessibility_analyzer.hpp"
#include <cmath>
#include <vector>
#include <string>
#include <numeric>
#include <tuple> 

using namespace ambulance_module;

ScAddr FindProblemZonesAgent::GetActionClass() const
{
  return AmbulanceKeynodes::action_find_problem_zones;
}

ScResult FindProblemZonesAgent::DoProgram(ScAction & action)
{
    ScAddr const actionNode = action;

    ScAddr optimalStation;
    bool stationFound = false;

    ScIterator3Ptr itOpt = m_context.CreateIterator3(
        AmbulanceKeynodes::nrel_optimal_location,  
        ScType::ConstPermPosArc,                  
        ScType::Unknown                         
    );

    while (itOpt->Next())
    {
        ScAddr resultArc = itOpt->Get(2);  
        
        // узнаюм какие элементы соединяют эту дугу
        auto [source, target] = m_context.GetConnectorIncidentElements(resultArc);
        // source - откуда идет связь (обычно действие)
        // target - куда идет связь (деревня-кандидат)
        
        // 6. ПРОВЕРЯЕМ, ЯВЛЯЕТСЯ ЛИ target ДЕРЕВНЕЙ
        if (m_context.CheckConnector(
            AmbulanceKeynodes::concept_village,  
            target,                              
            ScType::ConstPermPosArc))            
        {
            optimalStation = target;  
            stationFound = true;      
            break;                    
        }
    }

    if (!stationFound)
    {
        m_logger.Error("Optimal station not found. Run FindOptimalAgent first.");
        return action.FinishWithError();
    }

    // чтение координат из памяти
    auto GetCoord = [&](ScAddr const & node, ScAddr const & rel) -> double {
        ScIterator5Ptr it = m_context.CreateIterator5(
            node, ScType::ConstCommonArc, ScType::NodeLink,
            ScType::ConstPermPosArc, rel
        );
        
        if (it->Next()) {
            std::string str;
            m_context.GetLinkContent(it->Get(2), str);
            try { 
                return std::stod(str); 
            } catch(...) { 
                return 0.0; 
            }
        }
        return 0.0;
    };

    double optX = GetCoord(optimalStation, AmbulanceKeynodes::nrel_coordinate_x);
    double optY = GetCoord(optimalStation, AmbulanceKeynodes::nrel_coordinate_y);

    
    struct VillageDist { ScAddr addr; double dist; };
    
    std::vector<VillageDist> list;  // Список всех деревень с расстояниями
    double totalDist = 0;           // Сумма всех расстояний

    ScIterator3Ptr itV = m_context.CreateIterator3(
        AmbulanceKeynodes::concept_village,
        ScType::ConstPermPosArc,
        ScType::Unknown
    );
    
    // перебираем все деревни
    while(itV->Next())
    {
        ScAddr v = itV->Get(2); 
        
        if (v == optimalStation) continue; 
        
        double x = GetCoord(v, AmbulanceKeynodes::nrel_coordinate_x);
        double y = GetCoord(v, AmbulanceKeynodes::nrel_coordinate_y);
        
        double d = std::hypot(x - optX, y - optY);
        
        list.push_back({v, d});
        totalDist += d; 
    }

    //если нет других деревень - завершаем
    if (list.empty()) return action.FinishSuccessfully();

    //вычисляем среднее расстояние
    double avgDist = totalDist / list.size();
    
    //порог для проблемных зон
    double threshold = avgDist * 1.5;  // 150% от среднего
    

    m_logger.Info("Average distance: " + std::to_string(avgDist) + 
                ", Threshold: " + std::to_string(threshold));

    //структура для результатов
    ScStructure resultStruct = m_context.GenerateStructure();

    // ищем и помечаем проблемные зоны
    for (const auto& item : list)
    {
        
        if (item.dist > threshold)
        {
            //создание связи действие - проблемная зона
            ScAddr resArc = m_context.GenerateConnector(
                ScType::ConstCommonArc,  
                actionNode,              
                item.addr                
            );
            
            //помечаем связь как проблемная деревня
            m_context.GenerateConnector(
                ScType::ConstPermPosArc,           
                AmbulanceKeynodes::nrel_problem_zone,  
                resArc                            
            );
            
      
            resultStruct << item.addr 
                        << resArc 
                        << AmbulanceKeynodes::nrel_problem_zone;
            
           
            m_logger.Info("Problem zone found! Distance: " + std::to_string(item.dist));
        }
    }

    action.SetResult(resultStruct);
    return action.FinishSuccessfully();
}
