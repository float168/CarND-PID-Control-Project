#include <math.h>
#include <uWS/uWS.h>
#include <iostream>
#include <string>
#include <fstream>
#include "json.hpp"
#include "pid.hpp"

// for convenience
using nlohmann::json;
using std::string;

// For converting back and forth between radians and degrees.
constexpr double pi() { return M_PI; }
double deg2rad(double x) { return x * pi() / 180; }
double rad2deg(double x) { return x * 180 / pi(); }

// Checks if the SocketIO event has JSON data.
// If there is data the JSON object in string format will be returned,
// else the empty string "" will be returned.
string hasData(string s) {
  auto found_null = s.find("null");
  auto b1 = s.find_first_of("[");
  auto b2 = s.find_last_of("]");
  if (found_null != string::npos) {
    return "";
  }
  else if (b1 != string::npos && b2 != string::npos) {
    return s.substr(b1, b2 - b1 + 1);
  }
  return "";
}

int main(int argc, char** argv) {
  uWS::Hub h;

  if (argc > 1) {
    auto s = std::string(argv[1]);
    if (s == "--help" || s == "-h" || argc <= 3) {
      std::cerr << "usage: " << argv[0] << " kp ki kd" << std::endl;
      exit(EXIT_FAILURE);
    }
  }

  const double kp = argc > 3 ? std::strtod(argv[1], nullptr) : 0.3;
  const double ki = argc > 3 ? std::strtod(argv[2], nullptr) : 0.0006;
  const double kd = argc > 3 ? std::strtod(argv[3], nullptr) : 1.2;
  PID pid(kp, ki, kd);

  constexpr uint64_t total_count = 2400;
  uint64_t count = 0;
  h.onMessage([&pid,&count,kp,ki,kd](uWS::WebSocket<uWS::SERVER> ws,
                                     char *data,
                                     size_t length,
                                     uWS::OpCode opCode) {
    // "42" at the start of the message means there's a websocket message event.
    // The 4 signifies a websocket message
    // The 2 signifies a websocket event
    if (length && length > 2 && data[0] == '4' && data[1] == '2') {
      auto s = hasData(string(data).substr(0, length));

      if (s != "") {
        auto j = json::parse(s);

        string event = j[0].get<string>();

        if (event == "telemetry") {
          // j[1] is the data JSON object
          double cte = std::stod(j[1]["cte"].get<string>());
          double speed = std::stod(j[1]["speed"].get<string>());
          double angle = std::stod(j[1]["steering_angle"].get<string>());

          pid.UpdateCTE(cte);
          double steer_value = pid.CalcSteeringValue(speed, angle);

          // DEBUG
          std::cout << "[" << count << "] " << "CTE: " << cte
            << " Steering Value: " << steer_value << std::endl;

          if (++count == total_count) {
            const std::string fname = "total_error.txt";
            bool exists = std::ifstream(fname) ? true : false;

            std::ofstream f("total_error.txt", std::ios_base::app);
            if (!exists) {
              f << "kp ki kd total_cte count\n";
            }

            f << kp << " "<< ki << " " << kd
              << " " << pid.GetTotalError() << " " << count << std::endl;
          }

          json msgJson;
          msgJson["steering_angle"] = steer_value;
          msgJson["throttle"] = 0.3;
          auto msg = "42[\"steer\"," + msgJson.dump() + "]";
          std::cout << msg << std::endl;
          ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
        }  // end "telemetry" if
      } else {
        // Manual driving
        string msg = "42[\"manual\",{}]";
        ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
      }
    }  // end websocket message if
  }); // end h.onMessage

  h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
    std::cout << "Connected!!!" << std::endl;
  });

  h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code, 
                         char *message, size_t length) {
    ws.close();
    std::cout << "Disconnected" << std::endl;
  });

  int port = 4567;
  if (h.listen(port)) {
    std::cout << "Listening to port " << port << std::endl;
  } else {
    std::cerr << "Failed to listen to port" << std::endl;
    return -1;
  }
  
  h.run();
}
