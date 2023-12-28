
//
// This is example code from Chapter 16.5 "An example" of
// "Programming -- Principles and Practice Using C++" by Bjarne Stroustrup
//

#include <iostream>
#include <sstream>
#include "Graph.h"        // get access to our graphics library facilities
#include "GUI.h"
#include "Window.h"
#include "Simple_window.h"
#include <iomanip>
//#include "highlight.h"

using namespace Graph_lib;



//------------------------------------------------------------------------------

struct Lines_window :   Simple_window {
    Lines_window(Point xy, int w, int h, const string& title );
    Open_polyline lines;
    vector<Point> points;
    Graph_lib::highlight_poly selected;

private:
    Button next_button;        // add (next_x,next_y) to lines
    Button quit_button;
    Button clear_button;
    Button finish_button;
    In_box next_x;
    In_box next_y;
    In_box plo1;
    In_box plo2;
    Out_box xy_out;
    Out_box xy_first;
    Out_box xy_clicked;
    Out_box pixel_area;
    Out_box real_area;
    Text m_text;
    Text m2_text;
    Image map_attach;
    double rate;

    static void cb_next(Address, Address); // callback for next_button
    void next();
    static void cb_quit(Address, Address); // callback for quit_button
    void quit();
    static void cb_clear(Address, Address); // callback for quit_button
    void clear();
    static void cb_finish(Address, Address); // callback for quit_button
    void finish();

    int handle(int event) override {
        if (event == FL_PUSH) {
            int x = Fl::event_x();
            int y = Fl::event_y();

            // 判断点击位置是否在按钮范围内
            if (y>=55) {
                // 处理鼠标点击窗口其他区域的事件
                std::ostringstream oss;
                oss << "(" << x << "," << y << ")";
                std::string s3 = oss.str();
                xy_clicked.put(s3);
                // 在这里可以执行你想要的其他操作
            }
        }

        // 调用基类的事件处理函数，确保保留按钮的点击回调
        return Fl_Window::handle(event);
    }


};



//------------------------------------------------------------------------------

Lines_window::Lines_window(Point xy, int w, int h, const string& title)
        :Simple_window(xy,w,h,title),
         next_button(Point(x_max()-150,0), 70, 20, "Next point", cb_next),
         quit_button(Point(x_max()-70,0), 70, 20, "Quit", cb_quit),
         finish_button(Point(x_max()-150,30), 70, 20, "finish", cb_finish),
         clear_button(Point(x_max()-70,30), 70, 20, "clear", cb_clear),
         next_x(Point(x_max()-340,0), 50, 20, "next x:"),
         next_y(Point(x_max()-240,0), 50, 20, "next y:"),
         xy_out(Point(100,0), 90, 20, "current (x,y):"),
         xy_first(Point(445,0), 100, 20, "start (x,y):"),
         plo1(Point(x_max()-300,30), 50, 20, "Plotting Scale"),
         plo2(Point(x_max()-240,30), 50, 20, ":"),
         m_text(Point(x_max()-190,48),"m"),
         m2_text(Point(446,48),"m^2"),
         xy_clicked(Point(280,0), 90, 20, "clicked (x,y):"),
         pixel_area(Point(100,30), 90, 20, "pixel area:"),
         real_area(Point(280,30),160, 20, "real area:"),
         map_attach(Point((1400-w)/2,55), "hua_shi.jpg")
{
    selected.set_color(FL_YELLOW);
    lines.set_color(FL_RED);
    attach(next_button);
    attach(quit_button);
    attach(finish_button);
    attach(clear_button);
    attach(next_x);
    attach(next_y);
    attach(xy_out);
    attach(xy_first);
    attach(xy_clicked);
    attach(pixel_area);
    attach(real_area);

    attach(plo1);
    attach(plo2);
    attach(m_text);
    attach(m2_text);
    map_attach.change_scale(1400,800);
    attach(map_attach);


    attach(lines);
//    attach(selected);
}

//------------------------------------------------------------------------------

void Lines_window::cb_quit(Address, Address pw)    // "the usual"
{
    reference_to<Lines_window>(pw).quit();
}

//------------------------------------------------------------------------------

void Lines_window::quit()
{
    hide();        // curious FLTK idiom for delete window
}

//------------------------------------------------------------------------------

void Lines_window::cb_next(Address, Address pw)     // "the usual"
{
    reference_to<Lines_window>(pw).next();
}

//------------------------------------------------------------------------------

void Lines_window::next()
{
    int x = next_x.get_int();
    int y = next_y.get_int();

    lines.add(Point(x,y));
    selected.points.push_back(Point(x,y));


    points.push_back(Point(x,y));

    // update current position readout:
    stringstream ss;
    ss << '(' << x << ',' << y << ')';
    xy_out.put(ss.str());

    std::ostringstream oss;
    oss << "(" << lines.points[0].x << "," << lines.points[0].y << ")";
    std::string s2 = oss.str();
    xy_first.put(s2);

    redraw();
}

//------------------------------------------------------------------------------

void Lines_window::cb_clear(Address, Address pw)    // "the usual"
{
    reference_to<Lines_window>(pw).clear();
}

//------------------------------------------------------------------------------

void Lines_window::clear()
{
    detach(lines);
    detach(selected);
    lines.points.clear();
    selected.points.clear();
    attach(lines);
    xy_first.put(" ");
    xy_out.put(" ");
    xy_clicked.put(" ");
    pixel_area.put(" ");
    real_area.put(" ");
    redraw();        // curious FLTK idiom for delete window
}


void Lines_window::cb_finish(Address, Address pw)    // "the usual"
{
    reference_to<Lines_window>(pw).finish();
}

//------------------------------------------------------------------------------

void Lines_window::finish()
{
//    highlight_poly selected(points);
//    highlight_poly selected(points);
    double pixel_area_value = selected.calculate_area();
    std::stringstream ss;
    ss << std::scientific << std::setprecision(6) << pixel_area_value;
    std::string pixel_value = ss.str();
    pixel_area.put(std::to_string(pixel_area_value));

    int former = plo1.get_int();
    int latter = plo2.get_int();
    double rate = (latter*1.00)/(former*1.00);
    double real_area_value = pixel_area_value * pow(rate, 2);

    std::stringstream ss2;
    ss2 << std::scientific << std::setprecision(6) << real_area_value;
    std::string real_value = ss2.str();
    real_area.put(real_value);

    selected.set_color(FL_YELLOW);
    attach(selected);

    lines.add(lines.points[0]);
    redraw();        // curious FLTK idiom for delete window
}


//--------------------------------------------
int main()
try {
    Lines_window win(Point(50,50),1400,855,"fltk_map");
    Image baidu_map(Point(300,55), "TA_given_example.jpg");
    int w = baidu_map.get_width();
    int h = baidu_map.get_height();
    return gui_main();
}
catch(exception& e) {
    cerr << "exception: " << e.what() << '\n';
    return 1;
}
catch (...) {
    cerr << "Some exception\n";
    return 2;
}

//------------------------------------------------------------------------------





//#include <iostream>
//#include "Simple_window.h"    // get access to our window library
//#include "Graph.h"            // get access to our graphics library facilities
////#include "superellipse.h"
////#include "poly.h"
//#include <vector>
//#include <cmath>
//#include <iomanip>
//#include "highlight.h"
//#include <sstream>
//using namespace Graph_lib;
//
////int calculate_area(highlight_poly poly){
////    int numPoints =  poly.points.size();
////    double area = 0.0;
////
////    for (int i = 0; i < numPoints; ++i) {
////        int next = (i + 1) % numPoints;  // 下一个顶点的索引
////
////        area += (poly.points[i].x * points[next].y - points[next].x * points[i].y);
////    }
////
////    // 最后乘以 0.5，并取绝对值
////    area = 0.5 * std::fabs(area);
////
////    return area;
////}
//
////------------------------------------------------------------------------------
//
//struct Map_window : Graph_lib::Window {
//    Map_window(Point xy, int w, int h, const string& title );
//    vector<Point> points;
//private:
//    Button next_button;        // add (next_x,next_y) to lines
//    Button quit_button;
//    Button clear_button;
//    In_box next_x;
//    In_box next_y;
//    Out_box xy_out;
//    Out_box xy_first;
//    Out_box pixel_area;
//    Out_box real_area;
//
//
//    static void cb_next(Address, Address); // callback for next_button
//    void next();
//    static void cb_quit(Address, Address); // callback for quit_button
//    void quit();
//    static void cb_clear(Address, Address); // callback for clear_button
//    void clear();
//};
//
////------------------------------------------------------------------------------
//
//Map_window::Map_window(Point xy, int w, int h, const string& title)
//        :Window(xy,w,h,title),
//         next_button(Point(x_max()-150,0), 70, 20, "Next point", cb_next),
//         quit_button(Point(x_max()-70,0), 70, 20, "Quit", cb_quit),
//         clear_button(Point(x_max()-70,0), 70, 20, "clear", cb_clear),
//         next_x(Point(x_max()-310,0), 50, 20, "next x:"),
//         next_y(Point(x_max()-210,0), 50, 20, "next y:"),
//         xy_out(Point(100,0), 100, 20, "current (x,y):"),
//         xy_first(Point(100,30), 100, 20, "start (x,y):"),
//         pixel_area(Point(x_max()-450,30), 100, 20, "pixel area:"),
//         real_area(Point(x_max()-350,30), 100, 20, "real area:")
//
//{
//    attach(next_button);
//    attach(quit_button);
//    attach(clear_button);
//    attach(next_x);
//    attach(next_y);
//    attach(xy_out);
//    attach(xy_first);
//    attach(pixel_area);
//    attach(real_area);
////    attach(lines);
//}
//
////------------------------------------------------------------------------------
//
//void Map_window::cb_quit(Address, Address pw)    // "the usual"
//{
//    reference_to<Map_window>(pw).quit();
//}
//
////------------------------------------------------------------------------------
//
//void Map_window::quit()
//{
//    hide();        // curious FLTK idiom for delete window
//}
//
////------------------------------------------------------------------------------
//
//void Map_window::cb_next(Address, Address pw)     // "the usual"
//{
//    reference_to<Map_window>(pw).next();
//}
//
////------------------------------------------------------------------------------
//void Map_window::next()
//{
//    int x = next_x.get_int();
//    int y = next_y.get_int();
//
//    points.push_back(Point(x,y));
//
//    // update current position readout:
//    stringstream ss,ss1;
//    ss << '(' << x << ',' << y << ')';
//    xy_out.put(ss.str());
//    ss << '(' << points[0].x << ',' << points[0].y << ')';
//    xy_first.put(ss.str());
//
//    redraw();
//}
//
////-----------------------------------------------------------------------
//void Map_window::cb_clear(Address, Address pw)    // "the usual"
//{
//    reference_to<Map_window>(pw).quit();
//}
//
////----------------------------------------------------------------------------------
//
//void Map_window::clear()
//{
//
//    redraw();
//}
//
////------------------------------------------------------------------------------
//
//int main()
//{
//    Simple_window win(Point(100,100),1200,1000,"Baidu_Map");
////    Image baidu_map(Point(300,300), "hua_dong_shi_da.jpg");
////    win.attach(baidu_map);
//
//    return gui_main();
//}
//
//


//int main(){
//    using namespace Graph_lib;
//    Image baidu_map(Point(0,0), "hua_dong_shi_da.jpg");
//    int w = baidu_map.get_width();
//    int h = baidu_map.get_height();
//    std::cout << w << std::endl;
//    std::cout << h << std::endl;
//    Simple_window win(Point(100,100),600,800,"Baidu_Map");
//    win.attach(baidu_map);
//    win.set_label("Baidu_Map");
//    vector <Point> points_init;
//    int XX, YY;
//    std::cout << "请输入形如‘XX:YYm’的比例尺，例如32:200m" <<endl;
//    std::cout << "XX:" ;
//    std::cin >> XX;
//    std::cout << endl ;
//    std::cout << "YY:" ;
//    std::cin >> YY;
//    std::cout << "m" << endl;
//    std::cout << "现在请依次输入需要连接的各点，区域将以输入点的顺序顺次连接" << std::endl;
//    int x, y;
//    while (cin >> x >> y){
//        points_init.push_back(Point(x,y));
//    }
////    points_init.push_back(Point(100,100));
////    points_init.push_back(Point(200,200));
////    points_init.push_back(Point(300,100));
////    points_init.push_back(Point(600,700));
////    points_init.push_back(Point(500,800));
//    highlight_poly selected(points_init);
//    highlight_poly_outline selected_outline(points_init);
//    double pixel_area = selected.calculate_area();
////    std::cout << "area: " << pixel_area << std::endl;
//    selected.set_color(FL_YELLOW);
//    selected_outline.set_color(FL_RED);
//    win.attach(selected);
//    win.attach(selected_outline);
//
//    double rate = (YY*1.00)/(XX*1.00);
//    double real_area = pixel_area * pow(rate, 2);
//
//    std::cout << "像素面积：" << setprecision(6) << pixel_area << std::endl;
//    std::cout << "实际面积：" << setprecision(6) <<  real_area << "m^2" << std::endl;
//
//
//    win.wait_for_button();
//
//    return 0;
//}
