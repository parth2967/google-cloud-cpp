// Copyright 2020 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef GOOGLE_CLOUD_CPP_GOOGLE_CLOUD_PUBSUB_TOPIC_ADMIN_CONNECTION_H
#define GOOGLE_CLOUD_CPP_GOOGLE_CLOUD_PUBSUB_TOPIC_ADMIN_CONNECTION_H

#include "google/cloud/pubsub/connection_options.h"
#include "google/cloud/pubsub/internal/publisher_stub.h"
#include "google/cloud/pubsub/subscription.h"
#include "google/cloud/pubsub/topic.h"
#include "google/cloud/pubsub/version.h"
#include "google/cloud/internal/pagination_range.h"
#include "google/cloud/status_or.h"
#include <google/pubsub/v1/pubsub.pb.h>
#include <memory>

namespace google {
namespace cloud {
namespace pubsub {
inline namespace GOOGLE_CLOUD_CPP_PUBSUB_NS {

/**
 * An input range to stream Cloud Pub/Sub topics.
 *
 * This type models an [input range][cppref-input-range] of
 * `google::pubsub::v1::Topic` objects. Applications can make a
 * single pass through the results.
 *
 * [cppref-input-range]: https://en.cppreference.com/w/cpp/ranges/input_range
 */
using ListTopicsRange = google::cloud::internal::PaginationRange<
    google::pubsub::v1::Topic, google::pubsub::v1::ListTopicsRequest,
    google::pubsub::v1::ListTopicsResponse>;

/**
 * An input range to stream the Cloud Pub/Sub subscriptions of a topic.
 *
 * This type models an [input range][cppref-input-range] of
 * `std::string` objects. Applications can make a
 * single pass through the results.
 *
 * [cppref-input-range]: https://en.cppreference.com/w/cpp/ranges/input_range
 */
using ListTopicSubscriptionsRange = google::cloud::internal::PaginationRange<
    std::string, google::pubsub::v1::ListTopicSubscriptionsRequest,
    google::pubsub::v1::ListTopicSubscriptionsResponse>;

/**
 * A connection to Cloud Pub/Sub.
 *
 * This interface defines pure-virtual methods for each of the user-facing
 * overload sets in `PublisherClient`. That is, all of `PublisherClient`'s
 * `Publish()` overloads will forward to the one pure-virtual `Publish()` method
 * declared in this interface, and similar for `PublisherClient`'s other
 * methods. This allows users to inject custom behavior (e.g., with a Google
 * Mock object) in a `PublisherClient` object for use in their own tests.
 *
 * To create a concrete instance that connects you to the real Cloud Pub/Sub
 * service, see `MakeTopicAdminConnection()`.
 */
class TopicAdminConnection {
 public:
  virtual ~TopicAdminConnection() = 0;

  //@{
  /**
   * Define the arguments for each member function.
   *
   * Applications may define classes derived from `PublisherConnection`, for
   * example, because they want to mock the class. To avoid breaking all such
   * derived classes when we change the number or type of the arguments to the
   * member functions we define light weight structures to pass the arguments.
   */
  /// Wrap the arguments for `CreateTopic()`
  struct CreateTopicParams {
    google::pubsub::v1::Topic topic;
  };

  /// Wrap the arguments for `GetTopic()`
  struct GetTopicParams {
    Topic topic;
  };

  /// Wrap the arguments for `UpdateTopic()`
  struct UpdateTopicParams {
    google::pubsub::v1::UpdateTopicRequest request;
  };

  /// Wrap the arguments for `ListTopics`
  struct ListTopicsParams {
    std::string project_id;
  };

  /// Wrap the arguments for `DeleteTopic()`
  struct DeleteTopicParams {
    Topic topic;
  };

  /// Wrap the arguments for `ListTopicSubscriptions()`
  struct ListTopicSubscriptionsParams {
    std::string topic_full_name;
  };
  //@}

  /// Defines the interface for `TopicAdminClient::CreateTopic()`
  virtual StatusOr<google::pubsub::v1::Topic> CreateTopic(
      CreateTopicParams) = 0;

  /// Defines the interface for `TopicAdminClient::GetTopic()`
  virtual StatusOr<google::pubsub::v1::Topic> GetTopic(GetTopicParams) = 0;

  /// Defines the interface for `TopicAdminClient::UpdateTopic()`
  virtual StatusOr<google::pubsub::v1::Topic> UpdateTopic(
      UpdateTopicParams) = 0;

  /// Defines the interface for `TopicAdminClient::ListTopics()`
  virtual ListTopicsRange ListTopics(ListTopicsParams) = 0;

  /// Defines the interface for `TopicAdminClient::DeleteTopic()`
  virtual Status DeleteTopic(DeleteTopicParams) = 0;

  /// Defines the interface for `TopicAdminClient::ListTopicSubscriptions()`
  virtual ListTopicSubscriptionsRange ListTopicSubscriptions(
      ListTopicSubscriptionsParams) = 0;
};

/**
 * Returns an PublisherConnection object to work with Cloud Pub/Sub publisher
 * APIs.
 *
 * The `PublisherConnection` class is not intended for direct use in
 * applications, it is provided for applications wanting to mock the
 * `PublisherClient` behavior in their tests.
 *
 * @see `PublisherConnection`
 *
 * @param options (optional) configure the `PublisherConnection` created by
 *     this function.
 */
std::shared_ptr<TopicAdminConnection> MakeTopicAdminConnection(
    ConnectionOptions const& options = ConnectionOptions());

}  // namespace GOOGLE_CLOUD_CPP_PUBSUB_NS
}  // namespace pubsub

namespace pubsub_internal {
inline namespace GOOGLE_CLOUD_CPP_PUBSUB_NS {

std::shared_ptr<pubsub::TopicAdminConnection> MakeTopicAdminConnection(
    pubsub::ConnectionOptions const& options,
    std::shared_ptr<PublisherStub> stub);

}  // namespace GOOGLE_CLOUD_CPP_PUBSUB_NS
}  // namespace pubsub_internal
}  // namespace cloud
}  // namespace google

#endif  // GOOGLE_CLOUD_CPP_GOOGLE_CLOUD_PUBSUB_TOPIC_ADMIN_CONNECTION_H
