#include "parser.h"
#include "routingdb.h"
#include "tree.h"
#include "router.h"
#include <string>
#include <cstdlib>
#include <sstream>

using namespace std;

ifstream inputFile;
ofstream outputFile;

RoutingDB db;
Router    router;
void ShowSyntax()
{
    cout << "syntax: " << endl;
    cout << "./gr [input_file] [output_file] " << endl;
    cout << endl;

    cout << "example: " << endl;
    cout << "./gr input/adaptec1.capo70.2d.35.50.90.gr a1.out" << endl;

    cout << endl;
}

void HandleArgument(const int argc, char **argv)
{
    if (argc < 3)
    {
        ShowSyntax();
        exit(1);
    }

    int arg_no = 1;

    /* input file */
    inputFile.open(argv[arg_no], ios::in);
    if (!inputFile)
    {
        cerr << "Could not open input file: " << argv[arg_no] << endl;
        exit(1);
    }

    arg_no++;

    /* output file */
    outputFile.open(argv[arg_no], ios::out);
    if (!outputFile)
    {
        cerr << "Could not open output file: " << argv[arg_no] << endl;
        exit(1);
    }

    arg_no++;
}

int main(int argc, char **argv)
{
    HandleArgument(argc, argv);

    {
        /* Parser */
        cout << "[Parser]" << endl;
        Parser parser;
        parser.ReadISPD(inputFile);
    }
    cout << endl;

    /* =================================== */
    /* Show input information after parser */
    db.ShowInfo();

    /* =================================== */

    {
        cout << "[  Tree Construction (Net Decomposition)  ]" << endl;

        RoutingTree tree;
        tree.MinimumSpanningTreeConstruction();

        tree.ShowInfo();
        cout << endl;
    }

    /* =================================== */
    /* Show net(subnet) information after net decomposition */

    db.ShowNetInfo();
    /* =================================== */
    router.buildGraph(db.GetHoriGlobalTileNo(), db.GetVertiGlobalTileNo(), 
                      db.GetLayerHoriCapacity(0), db.GetLayerVertiCapacity(1));
    for(int i = 0; i < db.GetCapacityAdjustNo(); ++i){
        CapacityAdjust adj = db.GetCapacityAdjust(i);
        router.adjustCapacity(adj.GetGx1(), adj.GetGy1(), adj.GetGx2(), adj.GetGy2(), adj.GetReduceCapacity());
    }

    fstream ofs(argv[2], ios::out);
    for(int i = 0; i < db.GetNetNo(); ++i){
        stringstream ss;
        int length;
        Net& net = db.GetNetByPosition(i);
        for(int j = 0; j < net.GetSubNetNo(); ++j){
            cout << j << endl;
            SubNet& sn = net.GetSubNet(j);
            router.Dijkstra(sn.GetSourcePinGx(), sn.GetSourcePinGy());
            router.traceback(sn.GetTargetPinGx(), sn.GetTargetPinGy(), 0, length, ss);
        }
        ofs << net.GetName() << ' ' << net.GetPinUid(i) << ' ' << length << endl;
        // ofs << db.GetSubNet(i).G
        ofs << ss.str() << "!\n";
    }

    {
        cout << "[Verify]" << endl;
        char cmd[100];

        //sprintf(cmd, "./eval2008.pl %s %s", argv[1], argv[2]);
        sprintf(cmd, "./eval.pl %s %s", argv[1], argv[2]);
        cout << cmd << endl;
        system(cmd);
    }

    cout << endl;
    cout << endl;

    return 0;
}
