import {createApp, inject, ref} from 'vue'
import axios from 'axios'
import VueAxios from 'vue-axios'

const stateButton = {
    props: ['value'],
    emits: ['toggle'],
    setup(props, ctx) {
        function toggle() {
            ctx.emit('toggle', !props.value);
        }

        return {
            toggle
        };
    },
    template: `
          <button v-if="value" class="on" @click="toggle">ON</button>
          <button v-else class="off" @click="toggle">OFF</button>
        `
};

const relayPanel = {
    components: {
        'state-button': stateButton,
    },
    props: ['value'],
    setup(props) {
        const relay = ref(props.value);
        const editMode = ref(false);
        const editName = ref();

        function enterEditMode() {
            editMode.value = true;
            editName.value = relay.value.name;
        }

        function save() {
            axios
                .get('/modules/' + props.value.id + '?name=' + editName.value)
                .then((response) => {
                    relay.value = response.data;
                    editMode.value = false;
                });
        }

        function cancelEdit() {
            editMode.value = false;
        }

        function setValue(val) {
            axios
                .get('/modules/' + props.value.id + '?value=' + (val ? 1 : 0))
                .then((response) => {
                    relay.value = response.data;
                });
        }

        function setInitialValue(val) {
            axios
                .get('/modules/' + props.value.id + '?initialValue=' + (val ? 1 : 0))
                .then((response) => {
                    relay.value = response.data;
                });
        }

        return {
            relay,
            editMode,
            editName,
            enterEditMode,
            save,
            cancelEdit,
            setValue,
            setInitialValue
        };
    },
    template: `
          <div class="relay">
            <div class="col-name">
              <span v-if="!editMode" v-on:dblclick="enterEditMode()">{{ relay.name }}</span>
              <div v-else>
                <input type="text" v-model="editName"/>
                <button @click="save()">Save</button>
                <button @click="cancelEdit()">Cancel</button>
              </div>
            </div>
            <div class="col-initial-label">
              Init value
            </div>
            <div class="col-initial-btn">
              <state-button :value="relay.initialValue" @toggle="(v) => setInitialValue(v)"></state-button>
            </div>
            <div class="col-value-label">
              Value
            </div>
            <div class="col-value-btn">
              <state-button :value="relay.value" @toggle="(v) => setValue(v)"></state-button>
            </div>
          </div>`
};

const app = createApp({
    components: {
        'relay-panel': relayPanel
    },
    setup() {
        const axios = inject('axios')

        const relays = ref([]);

        function reload() {
            axios
                .get('/modules')
                .then((response) => {
                    relays.value = response.data;
                });
        }

        reload();

        return {
            reload,
            relays,
        }
    }
});
app.use(VueAxios, axios)
app.provide('axios', app.config.globalProperties.axios)
app.mount('#app')