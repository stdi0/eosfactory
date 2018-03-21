#pragma once

#include <string>
#include <vector>

#include <teoslib/control.hpp>

using namespace std;

namespace teos {
  namespace control {

    /**
     * @brief Delete opened locally walets.
     */
    class DaemonDeleteWallets : public  TeosControl
    {
      public:
        DaemonDeleteWallets();
    };

    /**
     * @brief Delete locally opened walets,#include <teoslib/control/config.hpp>
     * Usage: ./teos daemon delete_wallets
     */
    class DaemonDeleteWalletsOptions : public ControlOptions
    {
    public:
      DaemonDeleteWalletsOptions(int argc, const char **argv) 
        : ControlOptions(argc, argv) {}

    protected:
      const char* getUsage() {
        return R"EOF(
Delete locally opened walets.
Usage: ./teos daemon delete_wallets
)EOF";
      }

      TeosControl executeCommand() {
        return DaemonDeleteWallets();
      }  

      void printout(TeosControl command, variables_map &vm); 

    };

    /**
     * @brief Kill a running EOS node process.
     */
    class DaemonStop : public TeosControl
    {
    public:
      DaemonStop();
    };

    class DaemonStopOptions : public ControlOptions
    {
    public:
      DaemonStopOptions(int argc, const char **argv) 
        : ControlOptions(argc, argv) {}

    protected:
      const char* getUsage() {
        return R"EOF(
Kill any running EOS node process.
Usage: ./teos node kill
)EOF";
      }

      TeosControl executeCommand() {
        return DaemonStop();
      }

      void printout(TeosControl command, variables_map &vm){
        sharp() << "Daemon is stopped." << endl;
      }
    };

    /**
     * @brief Start a test EOSIO daemon if no one is running.
     * 
     */
    class DaemonStart : public TeosControl
    {
      void action();

    public:
      DaemonStart(
        bool resync_blockchain = false,
        string eosiod_exe = "",
        string genesis_json = "",
        string http_server_address = "",
        string data_dir = "")
      {
        reqJson_.put("resync-blockchain", resync_blockchain);
        reqJson_.put("eosiod_exe", eosiod_exe);
        reqJson_.put("genesis-json", genesis_json);
        reqJson_.put("http_server_address_", http_server_address);
        reqJson_.put("data-dir", data_dir);
        action();
      }

      DaemonStart(ptree reqJson){
        reqJson_ = reqJson;
        action();
      }
    };

    class DaemonStartOptions : public ControlOptions 
    {
    public:
      DaemonStartOptions(int argc, const char **argv) : ControlOptions(argc, argv) {}

    protected:
      const char* getUsage() {
        return R"EOF(
Start test EOS node.
Usage: ./teos node start [Options]
)EOF";
      }

      options_description  argumentDescription() {
        options_description od("");
        od.add_options()
          ("clear,c", "Clear chain database and block log.")
          ("skip,s", "Skip waiting for a block.");
            
        return od;
      }

      bool checkArguments(variables_map &vm) {
        bool ok = true;
        if(vm.count("clear")){
          reqJson_.put("resync-blockchain", true);
        } else {
          reqJson_.put("resync-blockchain", false);
        }
        if(vm.count("skip")){
          reqJson_.put("wait", false);
        } else {
          reqJson_.put("wait", true);
        }
        return ok;
      }

      TeosControl executeCommand() {
        return DaemonStart(reqJson_);
      } 

      void printout(TeosControl command, variables_map &vm) {
        if (vm.count("verbose") > 0) {
          output("eosiod exe file", "%s", command.reqJson_.get<string>("eosiod_exe").c_str());
          output("genesis state file", "%s", command.reqJson_.get<string>("genesis-json").c_str());
          output("server address", "%s", command.reqJson_.get<string>("http-server-address").c_str());
          output("config directory", "%s", command.reqJson_.get<string>("data-dir").c_str());
        }
      }
    };

  }
}