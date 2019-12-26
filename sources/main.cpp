//Copyright 2019 <Sralk>

#include <iostream>
#include "SharedPtr.hpp"

class Page{
public:
    std::string text;
    explicit Page(std::string str){
        this->text = str;
    }
    Page(){
        this->text = "";
    }

    Page &operator+(Page Pg){
        std::string str = this->text + Pg.text;
        static Page Pg2(str);
        return Pg2;
    }
    Page &operator=(Page Pg){
        this->text = Pg.text;
        return *this;
    }
    explicit operator bool() const {
        return this->text != "";
    }
};

int main() {
    Page Page1("Hello");
    Page Page2("World");
    Page Page3;
    Page3 = Page1 + Page2;
    std::cout << Page3.text;
    SharedPtr<int>balls;
    int red_ball = 10;
    SharedPtr<int>red_balls(&red_ball);
    balls = red_balls;
    //bool flag = !red_balls;
    //bool flagocheck = bool(red_balls);
    int *ptrredball = balls.get();
    std::cout << *ptrredball;
    SharedPtr< SharedPtr<int> >blue_balls(&balls);
    ptrredball = blue_balls->get();
    std::cout << *ptrredball;
    if (red_balls) {
        ptrredball = red_balls.get();
        std::cout << *ptrredball;
    }

    int total;
    //SharedPtr<int> jok = &total;
   // delete &balls;
    total = red_balls.use_count();
    std::cout << total;
}
