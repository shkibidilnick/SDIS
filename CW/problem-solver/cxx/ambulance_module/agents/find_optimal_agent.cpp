#include "find_optimal_agent.hpp"
#include <cmath>
#include <limits>
#include <vector>
#include <string>

using namespace ambulance_module;

ScAddr FindOptimalAgent::GetActionClass() const
{
    return AmbulanceKeynodes::action_find_optimal_station;
}


ScResult FindOptimalAgent::DoProgram(ScAction & action)
{
    // 1. СОХРАНЯЕМ АДРЕС ДЕЙСТВИЯ
    ScAddr const actionNode = action;
    
    // 2. СОЗДАЕМ СТРУКТУРУ ДЛЯ ХРАНЕНИЯ ДАННЫХ О ДЕРЕВНЯХ
    struct VillageData
    {
        ScAddr addr;    
        double x;       
        double y;       
        int population; 
    };
    
    std::vector<VillageData> villages;
    
    ScAddrVector allVillageNodes;

    // 3. ИЩЕМ ВСЕ ДЕРЕВНИ В ПАМЯТИ
    ScIterator3Ptr const it3 = m_context.CreateIterator3(
        AmbulanceKeynodes::concept_village, 
        ScType::ConstPermPosArc,            // Связь "является экземпляром"
        ScType::Unknown                     // Любой элемент, связанный как деревня
    );
    

    // 4. СОБИРАЕМ АДРЕСА ВСЕХ ДЕРЕВЕНЬ
    while (it3->Next())  // перебираем все тройки
    {
        ScAddr const villageAddr = it3->Get(2);// третий элемент тройки - деревня
        allVillageNodes.push_back(villageAddr);
    }

    if (allVillageNodes.empty())
    {
        m_logger.Error("No villages found in the knowledge base.");
        return action.FinishWithError();
    }

    for (ScAddr const & villageAddr : allVillageNodes)
    {
        double x = 0.0;
        double y = 0.0;
        int pop = 0;
        bool dataFound = true; 

        // 7. СОЗДАЕМ ЛЯМБДА-ФУНКЦИЮ ДЛЯ ЧТЕНИЯ ЗНАЧЕНИЙ, читает значение из ссылкки
        auto GetValue = [&](ScAddr const & rel) -> double {
            ScIterator5Ptr const it5 = m_context.CreateIterator5(
                villageAddr,            // Исходный элемент (деревня)
                ScType::ConstCommonArc, // Дуга общего вида
                ScType::NodeLink,       // Ссылка (хранит число)
                ScType::ConstPermPosArc,// Дуга принадлежности
                rel                     // Отношение (coordinate_x, coordinate_y или population)
            );
            

            if (it5->Next()) 
            {
                ScAddr const linkAddr = it5->Get(2);
                
                std::string content_str;
                m_context.GetLinkContent(linkAddr, content_str); //содержимое ссылки в строку переводим
                
                try
                {
                    return std::stod(content_str); //строку в число
                }
                catch (...)
                {
                    return 0.0;
                }
            }
            return -1.0; 
        };

        double valX = GetValue(AmbulanceKeynodes::nrel_coordinate_x);
        double valY = GetValue(AmbulanceKeynodes::nrel_coordinate_y);
        double valPop = GetValue(AmbulanceKeynodes::nrel_population);

        // проверяем все ли данные найлены
        if (valX == -1.0 || valY == -1.0 || valPop == -1.0)
        {
            m_logger.Warning("Incomplete data for village with addr: " + 
                           std::to_string(villageAddr.Hash()));
            continue;// если данные не полные пропускаем деревню
        }
        //после чего сохраням данные о деревни
        villages.push_back({villageAddr, valX, valY, (int)valPop});
    }

    //проверка валидации
    if (villages.empty())
    {
        m_logger.Error("No valid village data found.");
        return action.FinishWithError();
    }

    // 12. АЛГОРИТМ ПОИСКА ОПТИМАЛЬНОЙ СТАНЦИИ
    double minScore = std::numeric_limits<double>::max();
    
    ScAddr bestVillageAddr;  
    bool found = false;      

    for (const auto & candidate : villages)
    {
        double currentScore = 0.0;  // Счет для текущей деревни

        // 14. ДЛЯ КАЖДОГО КАНДИДАТА СЧИТАЕМ ВЗВЕШЕННУЮ СУММУ
        for (const auto & target : villages)
        {
            double dist = std::hypot(candidate.x - target.x, 
                                     candidate.y - target.y);
            currentScore += dist * target.population;
        }

        // 15. СРАВНИВАЕМ С ТЕКУЩИМ ЛУЧШИМ РЕЗУЛЬТАТОМ
        if (currentScore < minScore)
        {
            minScore = currentScore;      
            bestVillageAddr = candidate.addr;  
            found = true;                 
        }
    }

    // 16. ПРОВЕРЯЕМ, НАШЛИ ЛИ МЫ КАНДИДАТА
    if (!found)
    {
        return action.FinishWithError();
    }

    // создание результата в памяти
    ScStructure resultStructure = m_context.GenerateStructure();
   

    // создание связи действие-оптимальная_деревня 
    ScAddr const resultArc = m_context.GenerateConnector(
        ScType::ConstCommonArc,  // Тип дуги: общая дуга
        actionNode,              // Откуда: действие
        bestVillageAddr          // Куда: оптимальная деревня
    );

    //помечаем связь как оптимальное располодение
    ScAddr const relArc = m_context.GenerateConnector(
        ScType::ConstPermPosArc,             // Дуга принадлежности
        AmbulanceKeynodes::nrel_optimal_location,  // Отношение "оптимальное расположение"
        resultArc                            // Дуга, которую помечаем
    );

    resultStructure << bestVillageAddr 
                    << resultArc 
                    << relArc 
                    << AmbulanceKeynodes::nrel_optimal_location;


    action.SetResult(resultStructure);
    

    m_logger.Info("Optimal station found. Min weighted score: " + 
                  std::to_string(minScore));


    return action.FinishSuccessfully();
}