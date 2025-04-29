package com.example.orellanaproject2

import android.content.Intent
import android.os.Bundle
import android.util.Log
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import com.example.orellanaproject2.databinding.ActivityCadastroBinding
import com.google.firebase.auth.*

class CadastroActivity : AppCompatActivity() {

    private lateinit var binding: ActivityCadastroBinding
    private lateinit var auth: FirebaseAuth
    private val TAG = "CadastroActivity"

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState) // SEMPRE primeiro
        binding = ActivityCadastroBinding.inflate(layoutInflater)
        setContentView(binding.root)

        auth = FirebaseAuth.getInstance()

        binding.CadastroBtn.setOnClickListener {
            val name = binding.NameField.text.toString()
            val email = binding.EmailCadastro.text.toString()
            val password = binding.senhaCadastro.text.toString()
            val password2 = binding.senhaCadastro2.text.toString()

            when {
                name.isEmpty() -> {
                    binding.NameField.error = "Name Required !!!"
                    binding.NameField.requestFocus()
                }
                email.isEmpty() -> {
                    binding.EmailCadastro.error = "Email Required !!!"
                    binding.EmailCadastro.requestFocus()
                }
                password.isEmpty() -> {
                    binding.senhaCadastro.error = "Password Required !!!"
                    binding.senhaCadastro.requestFocus()
                }
                password2.isEmpty() -> {
                    binding.senhaCadastro2.error = "Confirm Password Required !!!"
                    binding.senhaCadastro2.requestFocus()
                }
                password != password2 -> {
                    binding.senhaCadastro2.error = "Passwords do not match!"
                    binding.senhaCadastro2.requestFocus()
                }
                else -> {
                    auth.createUserWithEmailAndPassword(email, password).addOnCompleteListener {
                        if (it.isSuccessful) {
                            Log.d(TAG, "Cadastro realizado com sucesso")
                            val user = auth.currentUser
                            startActivity(Intent(this, TelaPrincipalActivity::class.java))
                            finish()
                        } else {
                            Log.d(TAG, "Cadastro falhou", it.exception)
                            tratarErrosCadastro(it.exception)
                        }
                    }
                }
            }
        }
    }

    private fun tratarErrosCadastro(exception: Exception?) {
        when (exception) {
            is FirebaseAuthWeakPasswordException ->
                binding.senhaCadastro.error = "Senha muito fraca"
            is FirebaseAuthInvalidCredentialsException ->
                binding.EmailCadastro.error = "E-mail inválido"
            is FirebaseAuthUserCollisionException ->
                Toast.makeText(this, "E-mail já cadastrado", Toast.LENGTH_SHORT).show()
            else ->
                Toast.makeText(this, "Erro: ${exception?.message}", Toast.LENGTH_LONG).show()
        }
    }

    private fun updateUI(user: FirebaseUser?) {
        if (user != null) {
            startActivity(Intent(this, TelaPrincipalActivity::class.java))
            finish()
        }
    }

    override fun onStart() {
        super.onStart()
        val currentUser = auth.currentUser
        updateUI(currentUser)
    }
}
