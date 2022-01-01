import ecosphere.objects.base


class EcoInterfaceEntry:

    def set_return_type(self, value):
        self._return_type = value
    
    def set_arg_type(self, index, value):
        self._arg_types[index] = value

    def serialize(self, serializer):
        serializer.write_object(self._name)
        if self._return_type is None:
            serializer.write_message('ecosphere.object.interface.default')
        else:
            serializer.write_object(self._return_type)
        if self._has_varargs:
            serializer.write_vlq(1)
        else:
            serializer.write_vlq(0)
        serializer.write_vlq(len(self._arg_types))
        for at in self._arg_types:
            if at is None:
                serializer.write_message('ecosphere.object.interface.default')
            else:
                serializer.write_object(at)

    def __init__(self, name, arg_count, has_varargs):
        self._return_type = None
        self._name = name
        self._arg_types = [None] * arg_count
        self._has_varargs = has_varargs


class EcoInterface(ecosphere.objects.base.EcoObject):
    
    def add_entry(self, entry):
        self._entries.append(entry)
    
    def serialize(self, serializer):
        if not serializer.try_serialize_known_object(self):
            serializer.write_message('ecosphere.object.interface')
            serializer.write_vlq(serializer.add_object(self))
            serializer.write_vlq(len(self._entries))
            for entry in self._entries:
                entry.serialize(serializer)

    def __init__(self):
        super().__init__
        self._entries = list()
