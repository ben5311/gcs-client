#define GOOGLE_CLOUD_CPP_STORAGE_DEFAULT_UPLOAD_BUFFER_SIZE (8 * 1024 * 1024)

#include "boost/program_options.hpp"
#include "google/cloud/storage/client.h"
#include "google/cloud/storage/parallel_upload.h"
#include <iostream>

using namespace std;
namespace po = boost::program_options;
namespace gcs = ::google::cloud::storage;

int main(int argc, char* argv[]) {
    try {
        string sourcePath;
        string targetBucket;
        string targetPath;

        // Define command line options
        po::options_description desc("Allowed options");
        desc.add_options()
        ("help,h", "print this help message")
        ("sourcePath", po::value(&sourcePath)->required(), "the source path")
        ("targetBucket", po::value(&targetBucket)->required(), "the target Bucket")
        ("targetPath", po::value(&targetPath)->required(), "the target path")
        ;

        // Parse command line options
        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        if (vm.count("help")) {
            cout << desc << endl;
            return 0;
        }
        po::notify(vm);

        // Create a GCS client that uses default authentication and project id.
        // auto client = google::cloud::storage_experimental::DefaultGrpcClient();  // for gRPC API
        auto client = gcs::Client();

        // Perform parallel composite upload
        cout << "Starting upload from " << sourcePath << " to gs://" << targetBucket << "/" << targetPath << endl;
        auto prefix = gcs::CreateRandomPrefixName("tmp/parallel-composite-upload/");
        auto metadata = gcs::ParallelUploadFile(client, sourcePath, targetBucket, targetPath, prefix, false);
        if (!metadata) {
          throw runtime_error(metadata.status().message());
        }
        cout << "Uploaded " << sourcePath << " to gs://" << targetBucket << "/" << targetPath << endl;
        cout << "Full metadata: " << *metadata << endl;
        return 0;
    }
    catch(exception& e) {
        cerr << e.what() << endl;
        return 1;
    }
}
