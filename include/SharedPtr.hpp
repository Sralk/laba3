//Copyright 2019 <Sralk>

#include <atomic>
#include <cstddef>

template<class T> //если работа просто с типами, а не классами, то перед Т пишем typename
class SharedPtr
{
public:
    SharedPtr()
            : totalUse(new std::atomic<size_t>{1})
    {}

    explicit SharedPtr(T *ptr) //explict - запрещает неявное опред
            : pointer(ptr), totalUse(new std::atomic<size_t>{1})
    {}

    SharedPtr(const SharedPtr &shared)
            : totalUse(nullptr)
    {
        *this = shared;
    }

    SharedPtr(SharedPtr &&shared) noexcept // игнорирует ошибки
            : totalUse(nullptr)
    {
        *this = std::move(shared);
    }

    SharedPtr &operator=(const SharedPtr &shared)
    {
        if (&shared == this) {
            return *this;
        }

        this->~SharedPtr();

        pointer = shared.pointer;
        totalUse = shared.totalUse;
        (*totalUse)++;

        return *this;
    }

    SharedPtr &operator=(SharedPtr &&shared) noexcept
    {
        if (&shared == this) {
            return *this;
        }

        this->~SharedPtr();

        pointer = shared.pointer;
        totalUse = shared.totalUse;
        shared.pointer = nullptr;
        shared.totalUse = nullptr;

        return *this;
    }

    // проверяет, указывает ли указатель на объект
    explicit operator bool() const
    {
        return pointer != nullptr;
    }

    T &operator*() const
    {
        return *pointer;
    }

    T *operator->() const
    {
        return pointer;
    }

    T *get()
    {
        return pointer;
    }

    void reset()
    {
        *this = SharedPtr{};
    }

    void reset(T *ptr)
    {
        *this = SharedPtr{ptr};
    }

    void swap(SharedPtr &shared)
    {
        if (&shared == this) {
            return;
        }

        std::swap(pointer, shared.pointer);
        std::swap(totalUse, shared.totalUse);
    }

    // возвращает количество объектов SharedPtr, которые ссылаются на тот же управляемый объект
    [[nodiscard]] size_t use_count() const //[[nodiscard]] данные слово заставляет пользователя записывать результат данной ф-ии в переменную
    {
        return *totalUse;
    }

    ~SharedPtr()
    {
        if (!totalUse) {
            return;
        }

        if (--(*totalUse) == 0) {
            delete pointer;
            delete totalUse;
        }
    }

private:
    T *pointer = nullptr;
    std::atomic<size_t> *totalUse;
};