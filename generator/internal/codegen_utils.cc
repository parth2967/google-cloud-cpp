// Copyright 2020 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "generator/internal/codegen_utils.h"
// TODO(#4501) - fix by doing #include <absl/...>
#if _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4244)
#endif  // _MSC_VER
#include "absl/strings/str_cat.h"
#include "absl/strings/str_join.h"
#include "absl/strings/str_replace.h"
#include "absl/strings/str_split.h"
#if _MSC_VER
#pragma warning(pop)
#endif  // _MSC_VER
// TODO(#4501) - end
#include <google/protobuf/compiler/code_generator.h>
#include <cctype>
#include <string>

namespace google {
namespace cloud {
namespace generator_internal {

std::string GeneratedFileSuffix() { return ".gcpcxx.pb"; }

std::string LocalInclude(absl::string_view header) {
  return absl::StrCat("#include \"", header, "\"\n");
}

std::string SystemInclude(absl::string_view header) {
  return absl::StrCat("#include <", header, ">\n");
}

std::string CamelCaseToSnakeCase(absl::string_view input) {
  std::string output;
  for (auto i = 0U; i < input.size(); ++i) {
    if (input[i] != '_' && i + 2 < input.size()) {
      if (std::isupper(static_cast<unsigned char>(input[i + 1])) &&
          std::islower(static_cast<unsigned char>(input[i + 2]))) {
        absl::StrAppend(
            &output,
            std::string(1, std::tolower(static_cast<unsigned char>(input[i]))),
            "_");
        continue;
      }
    }
    if (input[i] != '_' && i + 1 < input.size()) {
      if ((std::islower(static_cast<unsigned char>(input[i])) ||
           std::isdigit(static_cast<unsigned char>(input[i]))) &&
          std::isupper(static_cast<unsigned char>(input[i + 1]))) {
        absl::StrAppend(
            &output,
            std::string(1, std::tolower(static_cast<unsigned char>(input[i]))),
            "_");
        continue;
      }
    }
    absl::StrAppend(
        &output,
        std::string(1, std::tolower(static_cast<unsigned char>(input[i]))));
  }
  return output;
}

std::string ServiceNameToFilePath(absl::string_view service_name) {
  std::vector<absl::string_view> components = absl::StrSplit(service_name, ".");
  absl::ConsumeSuffix(&components.back(), "Service");
  auto formatter = [](std::string* s, absl::string_view sv) {
    *s += CamelCaseToSnakeCase(sv);
  };
  return absl::StrJoin(components, "/", formatter);
}

std::string ProtoNameToCppName(absl::string_view proto_name) {
  return "::" + absl::StrReplaceAll(proto_name, {{".", "::"}});
}

std::vector<std::string> BuildNamespaces(
    std::map<std::string, std::string> const& vars, NamespaceType ns_type) {
  // vars["product_path"] is guaranteed to be present and properly formatted.
  std::string product_path = vars.find("product_path")->second;
  std::vector<std::string> v = absl::StrSplit(product_path, '/');
  auto name = *----v.end();
  std::string inline_ns = absl::AsciiStrToUpper(name) + "_CLIENT_NS";
  if (ns_type == NamespaceType::INTERNAL) {
    name = absl::StrCat(name, "_internal");
  }
  return {"google", "cloud", name, inline_ns};
}

StatusOr<std::vector<std::pair<std::string, std::string>>>
ProcessCommandLineArgs(std::string const& parameters) {
  std::cerr << "paramters: " << parameters << std::endl;
  std::vector<std::pair<std::string, std::string>> command_line_args;
  google::protobuf::compiler::ParseGeneratorParameter(parameters,
                                                      &command_line_args);

  auto product_path =
      std::find_if(command_line_args.begin(), command_line_args.end(),
                   [](std::pair<std::string, std::string> const& p) {
                     return p.first == "product_path";
                   });
  if (product_path == command_line_args.end()) {
    return Status(StatusCode::kInvalidArgument,
                  "--cpp_codegen_opt=product_path=<path> must be specified.");
  }
  if (product_path->second.front() == '/') {
    product_path->second = product_path->second.substr(1);
  }
  if (product_path->second.back() != '/') {
    product_path->second = absl::StrCat(product_path->second, "/");
  }
  return command_line_args;
}

}  // namespace generator_internal
}  // namespace cloud
}  // namespace google
