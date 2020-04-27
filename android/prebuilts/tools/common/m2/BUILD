filegroup(
    name = "protoc",
    srcs = select({
        ":darwin": ["repository/com/google/protobuf/protoc/3.0.0-beta-2/protoc-3.0.0-beta-2-osx-x86_64.exe"],
        ":windows": ["repository/com/google/protobuf/protoc/3.0.0-beta-2/protoc-3.0.0-beta-2-windows-x86_64.exe"],
        "//conditions:default": ["repository/com/google/protobuf/protoc/3.0.0-beta-2/protoc-3.0.0-beta-2-linux-x86_64.exe"],
    }),
    visibility = ["//visibility:public"],
)

filegroup(
    name = "grpc-plugin",
    srcs = select({
        ":darwin": ["repository/io/grpc/protoc-gen-grpc-java/0.13.2/protoc-gen-grpc-java-0.13.2-osx-x86_64.exe"],
        ":windows": ["repository/io/grpc/protoc-gen-grpc-java/0.13.2/protoc-gen-grpc-java-0.13.2-windows-x86_64.exe"],
        "//conditions:default": ["repository/io/grpc/protoc-gen-grpc-java/0.13.2/protoc-gen-grpc-java-0.13.2-linux-x86_64.exe"],
    }),
    visibility = ["//visibility:public"],
)

filegroup(
    name = "guava-18",
    srcs = glob(["repository/com/google/guava/guava/18.0/**"]),
    visibility = ["//visibility:public"],
)

config_setting(
    name = "darwin",
    values = {"host_cpu": "darwin"},
    visibility = ["//visibility:public"],
)

config_setting(
    name = "windows",
    values = {"host_cpu": "x64_windows"},
    visibility = ["//visibility:public"],
)
