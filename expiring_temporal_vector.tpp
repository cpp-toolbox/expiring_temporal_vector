#ifndef EXPIRING_TEMPORAL_VECTOR_TPP
#define EXPIRING_TEMPORAL_VECTOR_TPP

#include <algorithm>
#include <iterator>

template <typename T>
ExpiringTemporalVector<T>::ExpiringTemporalVector(int time_limit_seconds)
    : time_limit_(std::chrono::seconds(time_limit_seconds)), current_index_(0) {}

template <typename T>
void ExpiringTemporalVector<T>::push_back(const T& element) {
    // Add the element to the vector
    elements_.push_back(element);

    // Store the current timestamp for this element
    timestamps_[current_index_] = Clock::now();

    // Increment the index for the next element
    ++current_index_;

    // Clean expired elements after adding a new one
    clean_expired_elements();
}

template <typename T>
std::vector<T> ExpiringTemporalVector<T>::get_elements() {
    std::vector<T> valid_elements;

    // Iterate through the elements and select those that are still valid
    for (size_t i = 0; i < elements_.size(); ++i) {
        if (Clock::now() - timestamps_[i] <= time_limit_) {
            valid_elements.push_back(elements_[i]);
        }
    }

    return valid_elements;
}

template <typename T>
std::vector<T> ExpiringTemporalVector<T>::get_elements_since(TimePoint time) {
    std::vector<T> valid_elements;

    // Iterate through the elements and select those whose timestamp is >= time
    for (size_t i = 0; i < elements_.size(); ++i) {
        if (timestamps_[i] >= time) {
            valid_elements.push_back(elements_[i]);
        }
    }

    return valid_elements;
}

template <typename T>
typename std::vector<T>::iterator ExpiringTemporalVector<T>::begin() {
    return elements_.begin();
}

template <typename T>
typename std::vector<T>::iterator ExpiringTemporalVector<T>::end() {
    return elements_.end();
}

template <typename T>
void ExpiringTemporalVector<T>::clean_expired_elements() {
    // Iterate and remove elements whose timestamp has expired
    auto it = std::remove_if(elements_.begin(), elements_.end(),
                             [this, idx = 0](const T&) mutable {
                                 return Clock::now() - timestamps_[idx++] > time_limit_;
                             });

    elements_.erase(it, elements_.end());

    // Clean the timestamps map as well
    for (auto it = timestamps_.begin(); it != timestamps_.end();) {
        if (Clock::now() - it->second > time_limit_) {
            it = timestamps_.erase(it); // Remove expired entry
        } else {
            ++it;
        }
    }
}

#endif // EXPIRING_TEMPORAL_VECTOR_TPP
