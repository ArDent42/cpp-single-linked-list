#pragma once
#include <algorithm>
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
		[[nodiscard]] bool operator==(const BasicIterator<const Type> &rhs) const noexcept {
			return this->node_ == rhs.node_;
		}
		[[nodiscard]] bool operator!=(const BasicIterator<const Type> &rhs) const noexcept {
			return this->node_ != rhs.node_;
		}
		[[nodiscard]] bool operator==(const BasicIterator<Type> &rhs) const noexcept {
			return this->node_ == rhs.node_;
		}
		[[nodiscard]] bool operator!=(const BasicIterator<Type> &rhs) const noexcept {
			return this->node_ != rhs.node_;
		}
		BasicIterator& operator++() noexcept {
			node_ = node_->next_node;
			return *this;
		}
		BasicIterator operator++(int) noexcept {
			BasicIterator old(*this);
			node_ = node_->next_node;
			return old;
		}
		[[nodiscard]] reference operator*() const noexcept {
			return node_->value;
		}
		[[nodiscard]] pointer operator->() const noexcept {
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
		head_.next_node = new Node(value, head_.next_node);
		++size_;
	}
	void PopFront() {
		Node *temp_next = head_.next_node;
		head_.next_node = head_.next_node->next_node;
		--size_;
		delete temp_next;
	}
	Iterator InsertAfter(ConstIterator pos, const Type &value) {
		pos.node_->next_node = new Node(value, pos.node_->next_node);
		++size_;
		return Iterator { pos.node_->next_node };
	}
	Iterator EraseAfter(ConstIterator pos) noexcept {
		Node *temp_next = pos.node_->next_node;
		pos.node_->next_node = pos.node_->next_node->next_node;
		delete temp_next;
		--size_;
		return Iterator { pos.node_->next_node };
	}
	void Clear() noexcept {
		while (head_.next_node) {
			Node *new_next = head_.next_node->next_node;
			delete head_.next_node;
			head_.next_node = new_next;
		}
		size_ = 0;
	}
	~SingleLinkedList() {
		Clear();
	}
	size_t GetSize() const noexcept {
		return size_;
	}
	bool IsEmpty() const noexcept {
		if (size_ == 0) {
			return true;
		}
		return false;
	}
	void swap(SingleLinkedList &other) noexcept {
		size_t temp_size = size_;
		Node *temp_ptr = head_.next_node;
		size_ = other.size_;
		head_.next_node = other.head_.next_node;
		other.size_ = temp_size;
		other.head_.next_node = temp_ptr;
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
		SingleLinkedList<Type> list_reversed;
		SingleLinkedList<Type> list;
		for (const auto &element : container) {
			list_reversed.PushFront(element);
		}
		for (const auto &element : list_reversed) {
			list.PushFront(element);
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

