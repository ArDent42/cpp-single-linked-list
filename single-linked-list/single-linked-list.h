#pragma once
#include <algorithm>
#include <utility>
template<typename Type>
class SingleLinkedList {
	struct Node {
		Node() = default;
		Node(const Type &val, Node *next)
		:
				value(val)
						, next_node(next) {
		}
		Type value;
		Node *next_node = nullptr;
	};

	template<typename ValueType>
	class BasicIterator {
		friend class SingleLinkedList;
		explicit BasicIterator(Node *node) {
			node_ = node;
		}

	public:

		using iterator_category = std::forward_iterator_tag;
		using value_type = Type;
		using difference_type = std::ptrdiff_t;
		using pointer = ValueType*;
		using reference = ValueType&;

		BasicIterator() = default;
		BasicIterator(const BasicIterator<Type> &other) noexcept {
			node_ = other.node_;
		}
		BasicIterator& operator=(const BasicIterator &rhs) = default;

		template<typename T>
		[[nodiscard]] bool operator==(const BasicIterator<T> &rhs) const noexcept {
			return this->node_ == rhs.node_;
		}
		template<typename T>
		[[nodiscard]] bool operator!=(const BasicIterator<T> &rhs) const noexcept {
			return !(this->node_ == rhs.node_);
		}

		BasicIterator& operator++() noexcept {
			assert(this->node_);
			node_ = node_->next_node;
			return *this;
		}
		BasicIterator operator++(int) noexcept {
			BasicIterator old(*this);
			++*this;
			return old;
		}
		[[nodiscard]] reference operator*() const noexcept {
			assert(this->node_);
			return node_->value;
		}
		[[nodiscard]] pointer operator->() const noexcept {
			assert(this->node_);
			return &node_->value;
		}

	private:
		Node *node_ = nullptr;
	};

public:

	using value_type = Type;
	using reference = value_type&;
	using const_reference = const value_type&;
	using Iterator = BasicIterator<Type>;
	using ConstIterator = BasicIterator<const Type>;

	SingleLinkedList(std::initializer_list<Type> values) {
		CreateList(values);
	}
	SingleLinkedList(const SingleLinkedList &other) {
		assert(size_ == 0 && head_.next_node == nullptr);
		CreateList(other);
	}
	SingleLinkedList& operator=(const SingleLinkedList &rhs) {
		if (head_.next_node != rhs.head_.next_node) {
			SingleLinkedList<Type> temp_list(rhs);
			swap(temp_list);
		}
		return *this;
	}
	SingleLinkedList() {
		head_.next_node = nullptr;
	}
	void PushFront(const Type &value) {
		InsertAfter(before_begin(), value);
	}
	void PopFront() {
		if (size_) {
		EraseAfter(before_begin());
		}
	}
	Iterator InsertAfter(ConstIterator pos, const Type &value) {
		pos.node_->next_node = new Node(value, pos.node_->next_node);
		++size_;
		return Iterator { pos.node_->next_node };
	}
	Iterator EraseAfter(ConstIterator pos) noexcept {
		delete std::exchange(pos.node_->next_node, pos.node_->next_node->next_node);
		--size_;
		return Iterator { pos.node_->next_node };
	}
	void Clear() noexcept {
		while (head_.next_node) {
			PopFront();
		}
	}
	~SingleLinkedList() {
		Clear();
	}
	size_t GetSize() const noexcept {
		return size_;
	}
	bool IsEmpty() const noexcept {
		return size_ == 0;
	}
	void swap(SingleLinkedList &other) noexcept {
		std::swap(size_, other.size_);
		std::swap(head_.next_node, other.head_.next_node);
	}
	[[nodiscard]] Iterator begin() noexcept {
		return Iterator { head_.next_node };
	}
	[[nodiscard]] Iterator end() noexcept {
		return Iterator { nullptr };
	}
	[[nodiscard]] Iterator begin() const noexcept {
		return Iterator { head_.next_node };
	}
	[[nodiscard]] Iterator end() const noexcept {
		return Iterator { nullptr };
	}
	[[nodiscard]] ConstIterator cbegin() const noexcept {
		return ConstIterator { head_.next_node };
	}
	[[nodiscard]] ConstIterator cend() const noexcept {
		return ConstIterator { nullptr };
	}
	[[nodiscard]] Iterator before_begin() noexcept {
		return Iterator { &head_ };
	}
	[[nodiscard]] ConstIterator cbefore_begin() const noexcept {
		return ConstIterator { const_cast<Node*>(&head_) };
	}
	[[nodiscard]] Iterator before_begin() const noexcept {
		return Iterator { &head_ };
	}

private:
	template<typename T>
	void CreateList(const T &container) {
		SingleLinkedList<Type> list;
		auto last_element_iter = list.cbefore_begin();
		for (const auto &element : container) {
			last_element_iter = list.InsertAfter(last_element_iter, element);
		}
		swap(list);
	}
	Node head_;
	size_t size_ = 0;
};

template<typename Type>
void swap(SingleLinkedList<Type> &lhs, SingleLinkedList<Type> &rhs) noexcept {
	lhs.swap(rhs);
}

template<typename Type>
bool operator==(const SingleLinkedList<Type> &lhs, const SingleLinkedList<Type> &rhs) {
	if (lhs.GetSize() == rhs.GetSize()) {
		return std::equal(lhs.begin(), lhs.end(), rhs.begin());
	}
	return false;
}

template<typename Type>
bool operator!=(const SingleLinkedList<Type> &lhs, const SingleLinkedList<Type> &rhs) {
	return !(lhs == rhs);
}

template<typename Type>
bool operator<(const SingleLinkedList<Type> &lhs, const SingleLinkedList<Type> &rhs) {
	return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), lhs.end());
}

template<typename Type>
bool operator<=(const SingleLinkedList<Type> &lhs, const SingleLinkedList<Type> &rhs) {
	return !(rhs < lhs);
}

template<typename Type>
bool operator>(const SingleLinkedList<Type> &lhs, const SingleLinkedList<Type> &rhs) {
	return !(lhs <= rhs);
}

template<typename Type>
bool operator>=(const SingleLinkedList<Type> &lhs, const SingleLinkedList<Type> &rhs) {
	return !(lhs < rhs);
}

