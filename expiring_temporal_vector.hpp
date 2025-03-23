#ifndef EXPIRING_TEMPORAL_VECTOR_HPP
#define EXPIRING_TEMPORAL_VECTOR_HPP

#include <vector>
#include <chrono>
#include <unordered_map>

/**
 * @brief A vector-like container where elements expire after a certain time limit and can be retrieved by time.
 * @tparam T The type of elements stored in the container.
 */
template <typename T>
class ExpiringTemporalVector {
public:
    using Clock = std::chrono::steady_clock;
    using TimePoint = Clock::time_point;

    /**
     * @brief Constructor to initialize the expiring temporal vector with a time limit.
     * @param time_limit The time duration after which elements expire.
     */
    explicit ExpiringTemporalVector(int time_limit_seconds);

    /**
     * @brief Add an element to the vector with the current timestamp.
     * @param element The element to be added.
     */
    void push_back(const T& element);

    /**
     * @brief Get all elements currently in the vector.
     * @return A vector of elements, excluding those that are expired.
     */
    std::vector<T> get_elements();

    /**
     * @brief Get all elements with timestamps greater than or equal to the specified time.
     * @param time The time point to compare against.
     * @return A vector of elements with timestamps greater than or equal to the input time.
     */
    std::vector<T> get_elements_since(TimePoint time);

    // Iterator methods for non-const access
    typename std::vector<T>::iterator begin();
    typename std::vector<T>::iterator end();

private:
    /**
     * @brief Remove elements older than the specified time limit.
     */
    void clean_expired_elements();

    std::vector<T> elements_;  // Vector of elements
    std::unordered_map<int, TimePoint> timestamps_;  // Map from index to timestamp
    std::chrono::seconds time_limit_;
    int current_index_;  // Current index to track the next element
};

#include "expiring_temporal_vector.tpp"

#endif // EXPIRING_TEMPORAL_VECTOR_HPP
