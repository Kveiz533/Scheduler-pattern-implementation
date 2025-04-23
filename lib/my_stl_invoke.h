#include <utility>


namespace my_stl {
    template <typename Predicate, typename... Args>
    auto Invoke(Predicate&& predicate, Args&&... args) -> decltype(std::forward<Predicate>(predicate)(std::forward<Args>(args)...)) {
        return std::forward<Predicate>(predicate)(std::forward<Args>(args)...);
    }
    
    template <typename Method, typename Object, typename... Args>
    auto Invoke(Method&& method, Object&& object, Args&&... args) -> decltype((std::forward<Object>(object).*std::forward<Method>(method))(std::forward<Args>(args)...)) {
        return (std::forward<Object>(object).*std::forward<Method>(method))(std::forward<Args>(args)...);
    }
    
    template <typename Method, typename Object, typename... Args>
    auto Invoke(Method&& method, Object* object, Args&&... args) -> decltype((object->*std::forward<Method>(method))(std::forward<Args>(args)...))  {
        return (object->*std::forward<Method>(method))(std::forward<Args>(args)...);
    }

    template <typename Member, typename Object>
    auto Invoke(Member&& member, Object&& object) -> decltype(std::forward<Object>(object).*std::forward<Member>(member)) {
        return std::forward<Object>(object).*std::forward<Member>(member);
    }
    
    template <typename Member, typename Object>
    auto Invoke(Member&& member, Object* object) -> decltype(object->*std::forward<Member>(member)) {
        return object->*std::forward<Member>(member);
    }

}