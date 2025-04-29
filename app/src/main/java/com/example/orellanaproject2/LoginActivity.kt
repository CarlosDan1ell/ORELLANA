package com.example.orellanaproject2

import android.content.Intent
import android.os.Bundle
import android.util.Log
import android.widget.Toast
import androidx.activity.enableEdgeToEdge
import androidx.appcompat.app.AppCompatActivity
import androidx.core.view.ViewCompat
import androidx.core.view.WindowInsetsCompat
import com.example.orellanaproject2.databinding.ActivityLoginBinding
import com.google.firebase.auth.*

class LoginActivity : AppCompatActivity() {

    companion object {
        private const val TAG = "LoginActivity"
    }

    private lateinit var auth: FirebaseAuth
    private lateinit var binding: ActivityLoginBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityLoginBinding.inflate(layoutInflater)
        setContentView(binding.root) // Definir conteúdo antes de usar os views

        enableEdgeToEdge()
        ViewCompat.setOnApplyWindowInsetsListener(findViewById(R.id.main)) { v, insets ->
            val systemBars = insets.getInsets(WindowInsetsCompat.Type.systemBars())
            v.setPadding(systemBars.left, systemBars.top, systemBars.right, systemBars.bottom)
            insets
        }

        auth = FirebaseAuth.getInstance()

        // Botão para ir para Cadastro
        binding.SignUp.setOnClickListener {
            startActivity(Intent(this, CadastroActivity::class.java))
        }

        // Botão de login
        binding.SignInButton.setOnClickListener {
            val email = binding.loginField.text.toString()
            val password = binding.senhaField.text.toString()

            if (email.isEmpty()) {
                binding.loginField.error = "Email Required !!!"
                binding.loginField.requestFocus()
            } else if (password.isEmpty()) {
                binding.senhaField.error = "Password Required !!!"
                binding.senhaField.requestFocus()
            } else {
                auth.signInWithEmailAndPassword(email, password).addOnCompleteListener {
                    if (it.isSuccessful) {
                        Log.d(TAG, "User login Successful")
                        val user = auth.currentUser
                        Log.d("CADASTRO", "UID do usuário: ${user?.uid}")
                        updateUI(user)
                    } else {
                        Log.d(TAG, "User login Failed")
                        tratarErrosLogin(it.exception)
                    }
                }
            }
        }
    }

    private fun tratarErrosLogin(exception: Exception?) {
        when (exception) {
            is FirebaseAuthInvalidCredentialsException -> {
                Toast.makeText(this, "Credenciais inválidas!", Toast.LENGTH_LONG).show()
            }
            is FirebaseAuthUserCollisionException -> {
                Toast.makeText(this, "Este e-mail já está cadastrado.", Toast.LENGTH_LONG).show()
            }
            is FirebaseAuthWeakPasswordException -> {
                Toast.makeText(this, "Senha fraca!", Toast.LENGTH_LONG).show()
            }
            else -> {
                Toast.makeText(this, "Erro ao fazer login: ${exception?.message}", Toast.LENGTH_LONG).show()
            }
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
